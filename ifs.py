#
#  file:  ifs.py
#
#  Generate points for IFS fractals
#
#  RTK, 18-Jun-2017
#  Last update:  18-Jun-2017
#
###############################################################

import sys
import numpy as np


###############################################################
#  IFS
#
class IFS:
    """Iterated Function System fractals"""

    maps = {
        "circle": {"nmaps":4, "probs": [0.25, 0.25, 0.25, 0.25],
                   "maps": [[[.2929, -.2929, .5], [.2929, .2929, .5], [0,0,1]],
                            [[-.2929, .2929, .5], [-.2929, -.2929, .5], [0,0,1]],
                            [[.2929, .2929, .5], [-.2929, .2929, .5], [0,0,1]],
                            [[-.2929,-.2929,.5], [.2929,-.2929,.5], [0,0,1]]]},
        "dragon": {"nmaps":2, "probs": [0.5, 0.5],
                   "maps": [[[.5, -.5, 0], [.5, .5, 0], [0,0,1]],
                            [[-.5, -.5, 1], [.5,-.5,0], [0,0,1]]]},
        "fern":   {"nmaps":4, "probs":[0.75, 0.115, 0.115, 0.02],
                   "maps":[[[.81, .07, .12], [-.04, .84, .195], [0,0,1]],  
                               [[.18, -.25, .12], [.27, .23, .02], [0,0,1]],
                               [[.19, .275, .16], [.238, -.14, .12], [0,0,1]],
                               [[.0235, .087, .11], [.045, .1666, 0], [0,0,1]]]},
        "koch":   {"nmaps":4, "probs":[0.25,0.25,0.25,0.25],
                   "maps":[[[0.333, 0, 0], [0, 0.333, 0], [0,0,1]],
                               [[0.167, -.288, .333], [.288, .167, 0], [0,0,1]],
                               [[.167, .288, .5], [-.288, .167, .288], [0,0,1]],
                               [[.333, 0, .667], [0, .333, 0], [0,0,1]]]},
        "shell": {"nmaps":2, "probs":[0.955, 0.045],
                  "maps":[[[.96, .15,-.08], [-.15, .96, .0861], [0,0,1]],
                               [[.11, -.05, 1.11], [.05, .11, .8], [0,0,1]]]},
        "sierpinski": {"nmaps":3, "probs":[0.3333,0.3333,0.3333],
                  "maps":[[[.5, 0, 0], [0, .5, 0], [0,0,1]],
                               [[.5, 0, .5], [0, .5, 0], [0,0,1]],
                               [[.5, 0, .25], [0, .5, .5], [0,0,1]]]},
        "tree": {"nmaps":6, "probs":[.05, .05, .4, .05, .05, .4],
                  "maps":[[[.03, 0, -5e-3], [0, .48, 0], [0,0,1]],
                               [[.035, 0, .01], [0, -.44, .21], [0,0,1]],
                               [[.41, -.41, -0.1], [.41, .41, .23], [0,0,1]],
                               [[.03, 0, .01], [0, .48, 0], [0,0,1]],
                               [[.035,0, -7e-3], [0, .41, .02], [0,0,1]],
                               [[.41, .41, .01], [-.41, .41, .23], [0,0,1]]]}
    }


    #--------------------------------------------------------------
    #  Random
    #
    def Random(self):
        """Return a random number using the current generator"""

        if (self.gen == "good"):
            self.x = 16807*self.x % 2147483647
            m = 2147483647
        else:
            self.x = 259*self.x % 32768
            m = 32768

        return self.x/float(m+1)


    #--------------------------------------------------------------
    #  ChooseMap
    #
    def ChooseMap(self):
        """Use the current RNG to select a map"""

        r = self.Random()
        a = 0.0
        k = 0

        for i in range(self.nmaps):
            if (r > a):
                k = i
            a += self.probs[i]

        return k


    #--------------------------------------------------------------
    #  GeneratePoints
    #
    def GeneratePoints(self):
        """Generate the requested fractal points"""

        self.xy = np.zeros((self.npoints,3))

        #  Transient
        xy = np.array([self.Random(), self.Random(), 1.0])
        for i in xrange(100):
            m = self.maps[self.ChooseMap(),:,:]
            xy = np.dot(m,xy)
        
        #  Keep these
        for i in xrange(self.npoints):
            k = self.ChooseMap()
            m = self.maps[k,:,:]
            xy = np.dot(m,xy)
            self.xy[i,:] = [xy[0],xy[1],k]


    #----------------------------------------------------------
    #  StorePoints
    #
    def StorePoints(self):
        """Write the points to disk"""

        np.savetxt(self.outfile, self.xy)

    
    #----------------------------------------------------------
    #  GetPoints
    #
    def GetPoints(self):
        """Get the points"""

        return self.xy

    
    #----------------------------------------------------------
    #  __init__
    #
    def __init__(self, npoints, outfile, name, gen):
        """Constructor"""

        self.gen = gen
        self.x = 1  # always start at 1
        self.npoints = npoints
        self.outfile = outfile
        self.name = name
        self.nmaps = self.maps[name]["nmaps"]
        self.probs = np.array(self.maps[name]["probs"])
        self.maps = np.array(self.maps[name]["maps"])


###############################################################
#  main
#
def main():
    """Generate the requested fractal"""

    if (len(sys.argv) == 1):
        print
        print "ifs <gen> <points> <output> <fractal>"
        print
        print "  <gen>      - good|bad - generator to use"
        print "  <points>   - number of points to calculate"
        print "  <output>   - output text file: x  y  map"
        print "  <fractal>  - sierpinski|fern|tree|circle|shell|dragon|koch"
        print
        return

    gen = sys.argv[1].lower()
    npoints = int(sys.argv[2])
    outfile = sys.argv[3]
    name = sys.argv[4]

    if (name not in ["sierpinski","fern","tree","circle","shell","dragon","koch"]):
        print
        print "Unknown fractal: %s" % name
        print
        return

    app = IFS(npoints, outfile, name, gen)
    app.GeneratePoints()
    app.StorePoints()


if (__name__ == "__main__"):
    main()

