#
#  file:  bbs.py
#
#  Simple Blum Blum Shub implementation
#
#  RTK, 11-Jan-2018
#  Last update:  11-Jan-2018
#
###############################################################

import sys
import os

class BBS:
    #  period is numbthy.carmichael_lambda(numbthy.carmichael_lambda(1689529432697L)) = 23041980L
    #  p = 1299811, q = 1299827 
    #def __init__(self, seed, n=1689529432697L):

    #  period = lambda(lambda(2214251461768250569337)) = 375809633466223320 ~ 2^{58}
    #  p = 47055833443, q = 47055833459
    def __init__(self, seed, n=2214251461768250569337L):
        self.seed = seed
        self.n = n
        self.x0 = (seed*seed) % self.n
        self.l = 1107125730837069451218L
    def idx(self, m):
        b = 0
        for i in xrange(8*m, 8*m+8):
            e = pow(2, i, self.l)
            v = pow(self.x0, e, self.n)
            if (v % 2):
                b |= (1 << (i%8))
        return b
    def next(self):
        b = 0
        for i in xrange(8):
            self.seed = (self.seed*self.seed) % self.n
            if (self.seed % 2):
                b |= (1 << i)
        return b


def main():
    if (len(sys.argv) == 1):
        print
        print "bbs <seed> <n> <output>"
        print
        print "  <seed>   - uint32 seed value <47055833443"
        print "  <n>      - number of bytes to generate"
        print "  <output> - output file"
        print
        return

    bbs = BBS(int(sys.argv[1]))
    with open(sys.argv[3],"w") as f:
        for i in xrange(int(sys.argv[2])):
            f.write("%s" % chr(bbs.next()))

if (__name__ == "__main__"):
    main()

