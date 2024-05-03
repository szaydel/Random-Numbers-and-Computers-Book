import sys
import os

def pp(j,state):
    """Pretty print a state"""

    print "%04d:" % j,
    for i in range(16):
        print "1" if ((state>>(15-i))&1) else "0",
    print


def main():
    """Apply a rule to an initial state and evolve it"""

    if (len(sys.argv) == 1):
        print
        print "cellular_circle <rule> <16-bit state> <steps>"
        print
        print "  <rule>         -  rule as byte, in hex (0x42)"
        print "  <16-bit state> -  initial state, in hex (0xAAAA)"
        print "  <steps>        -  number of time steps"
        print
        return

    rule = int(sys.argv[1],16)
    state= int(sys.argv[2],16) & 0xFFFF
    steps= int(sys.argv[3])

    pp(0,state)
    for i in xrange(steps):
        nstate = 0
        for j in range(16):
            if (j == 0):
                if (rule & (1<<(((state&3)<<1)|((state>>15)&1)))):
                    nstate |= 1
            elif (j == 15):
                if (rule & (1<<(((state>>14)&3)|((state&1)<<2)))):
                    nstate |= (1<<15)
            else:
                if (rule & (1<<((state>>(j-1))&7))):
                    nstate |= (1<<j)
        state = nstate
        pp(i+1,state)


main()

