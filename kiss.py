#
#  file:  kiss.py
#
#  KISS32 in Python 
#
#  RTK, 21-Nov-2017
#  Last update:  21-Nov-2017
#
###############################################################

from xorshift32 import *

###############################################################
#  KISS
#
class KISS:
    """Encapsulate KISS"""

    #----------------------------------------------------------
    #  lcg32
    #
    def lcg32(self):
        """LCG part"""

        self.cng = ((69069*self.cng)&0xffffffff) + 13579
        self.cng &= 0xffffffff
        return self.cng

    
    #----------------------------------------------------------
    #  mwc32
    #
    def mwc32(self):
        """MWC part"""

        MASK = 0xffffffff
        self.j = ((self.j+1)&4194303)&MASK
        x = self.Q[self.j]
        t = (((x<<28)&MASK)+self.carry)&MASK
        self.carry = ((x>>4)&MASK)-(t<x)
        self.Q[self.j] = t-x
        return t-x


    #----------------------------------------------------------
    #  next
    #
    def next(self):
        """Next random 32-bit value"""

        n = self.mwc32() + self.x32.next() + self.lcg32()
        n &= 0xffffffff
        return n

    
    #----------------------------------------------------------
    #  next_fp
    #
    def next_fp(self):
        """Next random value as 53-bit float"""

        return float(self.next()) * 2.3283064359965952e-10

    
    #----------------------------------------------------------
    #  __init__
    #
    def __init__(self, seed=12345, stream=0):
        """Constructor"""

        self.cng = 123456789
        self.x32 = xorshift32(seed, stream=stream)
        self.carry = 0
        self.Q = [0]*4194304
        self.j = 4194303

        for i in xrange(4194304):
            self.Q[i] = self.lcg32() + self.x32.next()


