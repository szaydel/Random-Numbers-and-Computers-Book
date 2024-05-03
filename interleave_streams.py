#
#  file:  interleave_streams.py
#
#  Interleave streams so we can run normal statistical
#  tests on them.
#
#  RTK, 17-Nov-2017
#  Last update:  17-Nov-2017
#
###############################################################

import os
import sys

def main():
    """Interleave a set of streams"""

    if (len(sys.argv) == 1):
        print
        print "interleave_streams <outstream> <stream0> [<stream1> <stream2> ...]"
        print
        print "  <outstream>   - output stream file name"
        print "  <stream0> ... - stream file name(s)"
        print
        print "N.B. streams are assumed to be all of the same length"
        print
        return

    oname = sys.argv[1]
    fnames = sys.argv[2:]

    #  Open the files
    g = open(oname, "w")
    files = []
    for fname in fnames:
        files.append(open(fname))

    #  Interleave uint32 values
    b = os.stat(fnames[0]).st_size
    nsamp = b/4

    for i in xrange(nsamp):
        for k in range(len(files)):
            c = files[k].read(4)
            g.write(c)

    g.close()
    for f in files:
        f.close()


main()

