#
#  file:  ifs_plot.py
#
#  Plot a set of IFS points
#
#  RTK, 18-Jun-2017
#  Last update:  01-Jul-2017
#
###############################################################

import numpy as np
import matplotlib.pylab as plt
import sys


###############################################################
#  main
#
def main():
    """Plot the set of points"""

    if (len(sys.argv) == 1):
        print
        print "ifs_plot <points> [<output>]"
        print
        print "  <points>  -  text file of points"
        print "  <output>  -  if given, output plot name"
        print
        return

    points = np.loadtxt(sys.argv[1])

    if (len(sys.argv) < 3):
        output = ""
    else:
        output = sys.argv[2]

    colors = ['g','c','m','y','b','k','0.5']
    clr = []
    for i in range(points.shape[0]):
        clr.append(colors[int(points[i,2])])

    plt.axis("equal")
    plt.scatter(points[:,0], points[:,1], c=clr, marker=".", s=1)

    #  For good|bad fern example only
    #axes = plt.gca()
    #axes.set_xlim([0.1,0.3])
    #axes.set_ylim([0.05,0.25])

    if (output != ""):
        plt.savefig(output, dpi=1000)
    
    plt.show()


main()

