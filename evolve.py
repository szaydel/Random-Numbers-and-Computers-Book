#
#  File:  evolve.py
#
#  Run a GP session to evolve a PRNG.
#
#  RTK, 12-Dec-2017
#  Last update:  01-Jan-2018
#
###############################################################

from Func import *
#from Prog import *
from math import sqrt
import os
import sys
import time
import random
import struct
import copy
import cPickle
import numpy as np


###############################################################
#  Evolve
#
class Evolve:
    """Evolve a set of programs"""

    def ScoreSequence(self,s):
        """Score a generated sequence"""
        
        #  Compare to the  mask
        pmask = np.zeros((80,80))
        px = np.array([0,40,79], dtype="uint16")
        py = np.array([0,79,0], dtype="uint16")
        x = px[0]; y = py[0];

        for i in xrange(len(s)):
            n = int(3*(s[i]/65536.0))
            x = (x+px[n])/2
            y = (y+py[n])/2
            pmask[x,y]=1

        return np.sqrt(((pmask-self.mask)**2).sum())


    def RunProgram(self, v):
        """Evaluate returning the randomness measure"""

        x = 12345  # use the same seed each run
        s = []
        for i in range(self.ent):
            s.append(Func(v,x).Eval())
            x = s[-1]
        return self.ScoreSequence(s)


    def RandomProgram(self):
        """Create a random program"""

        #  Select random instructions.  For every dst selected,
        #  put it on a list and pull from the list randomly
        #  so that every dst is used at least once.
        p = []
        dlist = []
        n  = int(0.5*self.pmax)  # half
        n += int(0.5*random.random()*self.pmax)  # up to half
        
        for i in range(n):
            dst = int(10*random.random())  # dest reg
            dlist.append(dst)

            op  = int(11*random.random())  #  instr

            t = random.random()
            if (t < self.xprob):
                op0 = [0,-1]  # x
            elif (t < (self.xprob+self.rprob)):
                if (dlist == []):
                    op0 = [int(random.random()*10),+1]  # reg
                else:
                    op0 = [dlist[0],+1]
                    dlist = dlist[1:]
                    random.shuffle(dlist)
            else:
                op0 = [int(65536*random.random()),0]  # num

            t = random.random()
            if (t < self.xprob):
                op1 = [0,-1]  # x
            elif (t < (self.xprob+self.rprob)):
                if (dlist == []):
                    op1 = [int(random.random()*10),+1]  # reg
                else:
                    op1 = [dlist[0],+1]
                    dlist = dlist[1:]
                    random.shuffle(dlist)
            else:
                op1 = [int(65536*random.random()),0]  # num

            p += [op]+op0+op1+[dst]

        return p 

    
    def Msg(self, msg):
        """Track output"""

        self.msg += msg+"\n"
        print msg


    def CreateMaskArray(self):
        """Create the mask array"""

        self.mask = np.zeros((80,80))
        px = np.array([0,40,79], dtype="uint16")
        py = np.array([0,79,0], dtype="uint16")
        x = px[0]; y = py[0];

        for i in xrange(self.ent):
            n = random.randint(0,2)
            x = (x+px[n])/2
            y = (y+py[n])/2
            self.mask[x,y]=1


    def Initialize(self):
        """Build an initial population"""
        
        self.msg = ""

        if (self.verbose):
            self.Msg("")
            self.Msg("Genetic programming - evolve a 16-bit pseudorandom number generator")
            self.Msg("")
            self.Msg("Population size      : %d programs (max len=%d)" % (self.npop, self.pmax))
            self.Msg("Number of generations: %d" % self.ngen)
            self.Msg("breed top fraction   : %0.6f" % self.ftop)
            self.Msg("")

        #  Fill-in mask array
        self.CreateMaskArray()

        self.pop = []
        for i in range(self.npop):
            self.pop.append(self.RandomProgram())

        self.best_score = [1e38]
        self.best_prog = []


    def Breed(self, new_pop):
        """Breed these programs"""

        pop = []
        for i in range(len(new_pop)):
            #  Pick two programs
            p0 = random.randint(0,len(new_pop)-1)
            p1 = random.randint(0,len(new_pop)-1)
            while (p0 == p1):
                p1 = random.randint(0,len(new_pop)-1)

            #  Pick a break between instructions
            i0 = len(new_pop[p0])/6
            i1 = len(new_pop[p1])/6
            b0 = random.randint(1,i0-2)
            b1 = random.randint(1,i1-2)

            #  Create a new program
            prog = new_pop[p0][:(6*b0)] +  \
                   new_pop[p1][(6*b1):]

            #  Clip programs > 200% max length
            if (len(prog) > 2*6*self.pmax):
                prog = prog[:(2*6*self.pmax)]
            pop.append(prog)
        return pop


    def Step(self):
        """Do one eval/breed step"""

        #  Evaluate the current population and rank
        if (self.verbose):
            self.Msg("    evaluating and ranking current population")
        scores = np.zeros(len(self.pop))
        for i in range(len(self.pop)):
            scores[i] = self.RunProgram(self.pop[i])
        rank = np.argsort(scores)

        n = int(self.ftop*len(rank)+0.5)
        m = np.zeros(n)
        for i in range(n):
            m[i] = scores[rank[i]]
        mn = m.mean()
        se = m.std() / np.sqrt(n)

        if (self.verbose):
            self.Msg("    mean top scores = %0.10f +/- %0.10f" % (mn,se))

        #  Keep the best program ever found
        if scores[rank[0]] < self.best_score[-1]:
            if (self.best_prog == []):
                self.best_score = [scores[rank[0]]]
                self.best_prog = self.pop[rank[0]]
            else:
                self.best_score.append(scores[rank[0]])
                self.best_prog.append(self.pop[rank[0]])
            if (self.verbose):
                self.Msg("*** new best program found, score = %0.10f" % self.best_score[-1])

        new_pop = []
        if (se < self.se_min):
            #  Diversify the population
            if (self.verbose):
                self.Msg("    diversity too low, randomizing")
            for i in range(n/2):
                new_pop.append(self.pop[rank[i]])
            for i in range(n/2):
                new_pop.append(self.RandomProgram())
        else:
            #  Keep the top performing of this generation
            for i in range(n):
                new_pop.append(self.pop[rank[i]])

        #  Breed them to fill in the same fraction of
        #  the next generation
        if (self.verbose):
            self.Msg("    breeding next generation")
        new_pop += self.Breed(new_pop)

        #  Fill in any remaining with new random programs
        #  to keep the population of constant size
        if (len(new_pop) < len(self.pop)):
            if (self.verbose):
                self.Msg("    generating new random programs")
            for i in range(len(self.pop)-len(new_pop)):
                new_pop.append(self.RandomProgram())
            
        #  Make this the next generation
        self.pop = copy.deepcopy(new_pop)


    def Output(self):
        """Output results"""

        #  Report the best program
        if (self.verbose):
            self.Msg("")
            self.Msg("Search complete.  Best score = %0.10f" % self.best_score[-1])
            self.Msg("")
            self.Msg(Func(self.best_prog[-1],0).asCode())
            self.Msg("")
            self.Msg("(Runtime: %0.3f seconds)" % (self.end-self.start))
            self.Msg("")

        #  Other output
        os.system("rm -rf %s" % self.out)
        os.system("mkdir %s " % self.out)
        
        with open(self.out+os.sep+"parameters.txt","w") as f:
            f.write("Population size      : %d programs (max len=%d)\n" % (self.npop, self.pmax))
            f.write("Number of generations: %d\n" % self.ngen)
            f.write("breed top fraction   : %0.6f\n" % self.ftop)
            f.write("best program score   : %0.10f\n" % self.best_score[-1])
            f.write("runtime              : %0.3f\n" % (self.end-self.start))

        with open(self.out+os.sep+"gp_rand.c","w") as f:
            f.write(Func(self.best_prog[-1],0).asCode())

        with open(self.out+os.sep+"console.txt","w") as f:
            f.write(self.msg)

        with open(self.out+os.sep+"history_best_score.txt","w") as f:
            for s in self.best_score:
                f.write("%0.10f\n" % s)

        with open(self.out+os.sep+"history_best_programs.pkl","w") as f:
            cPickle.dump(self.best_prog,f)

        #  run.c
        with open(self.out+os.sep+"run.c","w") as f:
            s = """
#include <stdio.h>
#include <stdint.h>
uint16_t seed = 0x1234;

"""
            s += Func(self.best_prog[-1],0).asCode()
            s += """

void main(int argc, char *argv[]) {
    FILE *f;
    uint32_t i,nsamp,p;
    uint16_t s0,s1,s2,s3;
    uint16_t c0,c1,c2,c3;

    if (argc==1) {
        printf("\\nrun <seed> <nsamp> <output> <0|1>\\n\\n");
        printf("  <seed>   - uint16 seed\\n");
        printf("  <nsamp>  - number of uint16 samples\\n");
        printf("  <output> - output file name\\n");
        printf("  <0|1>    - 1=calculate period\\n\\n");
        return;
    }

    seed = (uint16_t)atoi(argv[1]);
    nsamp = (uint32_t)atol(argv[2]);
    f = fopen(argv[3],"w");

    for(i=0; i<nsamp; i++) {
        seed = gp_rand(seed);
        fwrite((void*)&seed, sizeof(uint16_t), 1, f);
    }

    fclose(f);

    if (atoi(argv[4])) {
        seed = 1;
        s0 = seed = gp_rand(seed);
        s1 = seed = gp_rand(seed);
        s2 = seed = gp_rand(seed);
        s3 = seed = gp_rand(seed);
        c0 = seed = gp_rand(seed);
        c1 = seed = gp_rand(seed);
        c2 = seed = gp_rand(seed);
        c3 = seed = gp_rand(seed);
        p = 4;

        while(1) {
            c0 = c1; c1 = c2; c2 = c3;
            c3 = seed = gp_rand(seed);
            p++;
            if ((s0==c0) && (s1==c1) && (s2==c2) && (s3==c3)) {
                printf("\\nperiod = %d\\n\\n", p);
                break;
            }
        }
    }
}
"""
            f.write(s)


    def Done(self):
        """Decide if done"""

        if (self.iteration >= self.ngen):
            return True

        return False


    def Run(self):
        """Run a simulation"""

        self.start = time.time()

        #  Construct an initial population
        self.Initialize()

        #  Iterate evolving along the way
        self.iteration = 0
        while (not self.Done()):
            self.Msg("Iteration #%d" % (self.iteration+1))
            self.Step()
            self.iteration += 1
            if (self.verbose):
                self.Msg("")

        self.end = time.time()


    def __init__(self, p=None, verbose=False):
        """Constructor"""

        self.verbose = verbose

        if (p is None):
            raise ValueError("Parameters must be given")

        self.ftop = p["top"]
        self.npop = p["pop"]
        self.pmax = p["len"]
        self.ngen = p["gen"]
        self.out  = p["out"]
        self.xprob= p["xprob"]
        self.rprob= p["rprob"]

        self.se_min = 1e-8  # diversify if below this threshold
        self.ent  = 10000   # number of mask points

        #  Never keep more that 50%
        if (self.ftop > 0.5):
            self.ftop = 0.5


###############################################################
#  main
#
def main():
    """Command line"""

    if (len(sys.argv) == 1):
        print
        print "evolve pop=<n>:<m> gen=<n> top=<f> out=<output>"
        print
        print "  pop  -  <n> programs of up to <m> instructions"
        print "  gen  -  <n> generations"
        print "  top  -  <f> breed top fraction (float)"
        print "  out  -  <output> directory (rewritten)"
        print
        return

    p = {
        "pop": 100,
        "len": 25,
        "gen": 100,
        "top": 0.33333,
        "out": "evolve_output",

        #  Adjust these manually here
        "xprob": 0.1,
        "rprob": 0.8,
    }

    for arg in sys.argv:
        if (arg[0:4]=="pop="):
            n,m = arg[4:].split(":")
            p["pop"]=int(n)
            p["len"]=int(m)
        elif (arg[0:4]=="gen="):
            p["gen"]=int(arg[4:])
        elif (arg[0:4]=="out="):
            p["out"]=arg[4:]
        elif (arg[0:4]=="top="):
            p["top"] = float(arg[4:])

    e = Evolve(p, verbose=True)
    e.Run()
    e.Output()


if (__name__ == "__main__"):
    main()


