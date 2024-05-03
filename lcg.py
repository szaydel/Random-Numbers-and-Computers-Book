#
#  file:  lcg.py
#
#  Simple LCG examples
#
#  RTK, 22-Jul-2017
#  Last update:  22-Jul-2017
#
###############################################################

def main():
    m = 10
    for a in range(1,m):
        for c in range(m):
            for s in range(m):
                x = s
                print "(a,c,s)=(%d,%d,%d): " % (a,c,s),
                for i in range(2*m):
                    x = (a*x+c) % m
                z = x
                first = True
                for i in range(2*m):
                    x = (a*x+c) % m
                    if (x == z) and (first):
                        p = i+1
                        first = False
                    print x,
                print " (period = %d)" % p


main()

