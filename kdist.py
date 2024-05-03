#
#  file:  kdist.py
#
#  Use the graphical interpretation of k-distributed
#  to explore the filling of space for MT with
#  v=1,2,3,4,5 and k=3.  Approximate since only using
#  10 million points (30 million calls)
#
#  RTK, 10-Aug-2017
#  Last update:  10-Aug-2017
#
###############################################################

import numpy as np
import cPickle

def main():
    """k-distribution example, k=3"""

    results = {}
    N = 100000000
    
    for v in range(1,6):
        h = np.zeros((2**v,2**v,2**v), dtype="uint32")

        for i in xrange(N):
            n = np.random.random(3)
            x0 = np.floor(n[0] / (2.0**(-v)))
            x1 = np.floor(n[1] / (2.0**(-v)))
            x2 = np.floor(n[2] / (2.0**(-v)))
            h[x0,x1,x2] += 1

        results[v] = h.reshape(2**v*2**v*2**v)

    cPickle.dump(results, open("kdist.pkl","w"))


main()

