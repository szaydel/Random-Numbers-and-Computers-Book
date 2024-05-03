#
#  file:  x32_streams.py
#
#  Simulate multiple streams
#
#  RTK, 14-Nov-2017
#  Last update:  19-Nov-2017
#
###############################################################

import os
import sys
import struct
import random
from xorshift32 import *


def main():
    """Simulate streams and generator output files"""

    if (len(sys.argv) == 1):
        print
        print "x32_streams <streams> <samples> <0|1> <prefix>"
        print
        print "  <streams> - number of streams (<82)"
        print "  <samples> - number of samples per stream"
        print "  <0|1>     - 0=default seed, 1=random seeds"
        print "  <base>    - output file base name"
        print
        return

    streams = int(sys.argv[1])
    samples = int(sys.argv[2])
    seedtype= int(sys.argv[3])
    base    = sys.argv[4]

    #  Make the "streams"
    g = []
    for i in range(streams):
        if (seedtype==1):
            s = xorshift32(seed=int((2**32-1)*random.random()), stream=i)
        else:
            s = xorshift32(stream=i)
        g.append(s)

    #  Run the streams
    for i in range(streams):
        with open(base+("_%02d.dat" % i),"w") as f:
            for j in range(samples):
                n = g[i].next()
                f.write(struct.pack("L",n))


main()

