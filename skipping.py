#
#  file:  skipping.py
#
#  Generate streams using skipping
#
#  RTK, 14-Nov-2017
#  Last update:  23-Nov-2017
#
###############################################################

import os
import sys
import struct
import random
from minstd import *
from xorshift128plus import *
from xorshift1024star import *

def main():
    if (len(sys.argv) == 1):
        print
        print "skipping <gen> <streams> <samples> <base>"
        print
        print "  <gen>      -  0=MINSTD, 1=xorshift128+, 2=xorshift1024*"
        print "  <streams>  -  number of streams (e.g. 10)"
        print "  <samples>  -  number of samples per stream (uint32)"
        print "  <base>     -  base name of output files"
        print
        return

    gen = int(sys.argv[1])
    streams = int(sys.argv[2])
    samples = int(sys.argv[3])
    base = sys.argv[4]

    #  Create the generators
    print "Creating the generators"
    g = []

    if (gen==0):
        seed = int((2**31-1)*random.random())
    else:
        seed = int((2**64-1)*random.random())

    for i in range(streams):
        if (gen == 0):
            s = MINSTD(A=16807)  # A=16807 orig, else revised
            s.init(seed)
            s.jump(int(1.1*samples*i))
            g.append(s)
        elif (gen == 1):
            s = xorshift128plus()
            s.init(seed)
            for j in range(i):
                s.jump()
            g.append(s)
        elif (gen == 2):
            s = xorshift1024star()
            s.init(seed)
            for j in range(i):
                s.jump()
            g.append(s)

    #  Run the streams
    print "Running the streams"
    for i in range(streams):
        with open(base+("_%03d.dat" % i), "w") as f:
            if (gen == 0):
                for j in range(samples):
                    n = int((2**32)*g[i].next_fp())
                    f.write(struct.pack("L",n)) 
            elif (gen == 1):
                for j in range(samples/2):
                    n = g[i].next()
                    f.write(struct.pack("Q",n))
            elif (gen == 2):
                for j in range(samples/2):
                    n = g[i].next()
                    f.write(struct.pack("Q",n))


main()

