#
#  file:  factorials_seq.py
#
#  Generate output binary files built from the
#  digits of factorials (minus trailing zeros)
#  for a sequence of factorials based on a starting number.
#
#  RTK, 28-Dec-2017
#  Last update:  28-Dec-2017
#
###############################################################

import sys
import os

def fact(n):
    """Factorial of n"""

    if (n<2):
        return 1
    s = 1
    for i in xrange(1,n+1):
        s *= i
    return s


def trailing(n):
    """Number of trailing zeros in n!"""

    s = 0
    while (n > 1):
        s += int(n/5.0)
        n = n/5.0
    return s


def main():
    """Create binary output files"""

    if (len(sys.argv) == 1):
        print
        print "factorials_seq <n> <m> <output>"
        print
        print "  <n>      - base factorial"
        print "  <m>      - number of following factorials"
        print "  <output> - output binary base filename"
        print
        return

    n = int(sys.argv[1])
    m = int(sys.argv[2])
    oname = sys.argv[3]

    for t in xrange(n,n+m):
        if (t == n):
            z = fact(t)
        else:
            z = t*z

        q = str(z)[:-(trailing(t))]
        if (len(q)%8 != 0):
            q = q[:-(len(q)%8)]
        
        with open(oname+"_%d" % t,"w") as f:
            k=0
            while (k < len(q)):
                b = 0
                for i in range(8):
                    v = 1 if int(q[k])>4 else 0
                    b |= v<<(7-i)
                    k += 1
                f.write("%s" % chr(b))


if (__name__ == "__main__"):
    main()

