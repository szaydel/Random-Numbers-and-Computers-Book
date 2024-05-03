import numpy as np
from math import floor

def halton(i,b):
    """Return i-th Halton number for the given base"""

    f = 1.0
    r = 0
    while (i > 0):
        f = f/b
        r = r + f*(i % b)
        i = floor(i/float(b))
    return r

def skip_halton(i,b,p):
    """Return the n-th Halton number using every multiple of prime p"""

    return halton(p*i,b)
        

def main():
    p2 = []
    p3 = []
    for i in range(500):
        p2.append(halton(i+1,17))
        p3.append(halton(i+1,31))

    p = np.zeros((500,2))
    p[:,0] = p2
    p[:,1] = p3
    np.savetxt("halton_2d.txt", p)

    p2 = []
    p3 = []
    for i in range(500):
        p2.append(skip_halton(i+1,17,40009))
        p3.append(skip_halton(i+1,31,40009))

    p = np.zeros((500,2))
    p[:,0] = p2
    p[:,1] = p3
    np.savetxt("halton_skip_40009_2d.txt", p)

    p = np.random.random((500,2))
    np.savetxt("halton_random_2d.txt", p)


if (__name__ == "__main__"):
    main()

