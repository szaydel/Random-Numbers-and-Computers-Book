#
#  file:  process_frames.py
#
#  Process captured frames to create
#  a pseudorandom sequence.
#
#  RTK, 20-Jun-2017
#  Last update:  20-Jun-2017
#
###############################################################

import sys
import os
import numpy as np
from scipy.misc import imresize


###############################################################
#  ProcessFrame
#
def ProcessFrame(data):
    """
    Generate pseudorandom numbers from an array representing
    a video frame
    """

    #  byte scale input
    m = data**2
    m = (m - m.min())/(m.max() - m.min())
    b = (255.0*m).astype("uint8")
    b = imresize(b,(500,500),interp="nearest")
    d = b.shape[0] if (b.shape[0] < b.shape[1]) else b.shape[1]
    h = []

    #  mix
    k = 0
    for i in range(d):
        for j in range(d):
            h.append(int(b[i,j]) ^ int(b[d-j-1,i]) ^ int(b[j,d-i-1]) ^ int(b[d-j-1,d-i-1]))

    #  Von Neumann whitening
    v = []
    vb = 0
    c = 0
    for n in h:
        for j in range(4):
            t = (n>>(2*j))&3
            if (t==2):
                vb |= (1<<c)
                c += 1
            elif (t==1):
                c += 1
            if (c > 7):
                v.append(vb)
                vb = 0
                c = 0
    
    return v


###############################################################
#  main
#
def main():
    """Process a set of frame files"""

    fdir = "frames_tv/"
    frames = [i for i in os.listdir(fdir) if i.find(".npy") != -1]
    v = []

    for f in frames:
        d = np.load(fdir+f)
        v += ProcessFrame(d[:,:,0])
        v += ProcessFrame(np.flipud(np.fliplr(d[:,:,1])))
        v += ProcessFrame(np.fliplr(np.flipud(d[:,:,2])))

    with open("random_tv.dat","w") as f:
        for i in xrange(len(v)):
            f.write(chr(v[i]))


if (__name__ == "__main__"):
    main()

