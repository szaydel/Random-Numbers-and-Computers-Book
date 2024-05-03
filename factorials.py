#
#  file:  factorials.py
#
#  Generate output binary files built from the
#  digits of factorials (minus trailing zeros)
#
#  RTK, 26-Dec-2017
#  Last update:  26-Dec-2017
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
        print "factorials <n> <output>"
        print
        print "  <n>      - factorial of n"
        print "  <output> - output binary file"
        print
        return

    n = int(sys.argv[1])
    oname = sys.argv[2]

    q = str(fact(n))[:-(trailing(n))]
    if (len(q)%8 != 0):
        q = q[:-(len(q)%8)]

    with open(oname,"w") as f:
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

