#
#  file:  xorshift128plus.py
#
#  xorshift128+ in Python 
#    (based on xorshift128plus.c by Sebastiano Vigna)
#
#  RTK, 13-Nov-2017
#  Last update:  14-Nov-2017
#
###############################################################

import struct

###############################################################
#  xorshift128plus
#
class xorshift128plus:
    """Encapsulate xorshift128+"""

    #----------------------------------------------------------
    #  next
    #
    def next(self):
        """Next random 64-bit value"""

        MASK = 0xffffffffffffffff
        s1 = self.x[0]
        s0 = self.x[1]
        result = (s0+s1) & MASK
        self.x[0] = s0 & MASK
        s1 ^= s1 << 23
        self.x[1] = (s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5)) & MASK
        return result

    
    #----------------------------------------------------------
    #  next_fp
    #
    def next_fp(self):
        """Next random value as 53-bit float"""

        n = (self.next() >> 12) | 0x3FF0000000000000
        d = struct.unpack("d", struct.pack("Q",n))[0] - 1.0
        return d

    
    #----------------------------------------------------------
    #  jump
    #
    def jump(self):
        """Jump ahead 2^64"""

        JUMP = [ 0x8a5cd789635d2dff, 0x121fd2155c472f96]
        s0 = s1 = 0
        for i in range(2):
            for b in range(64):
                if (JUMP[i] & (1<<b)):
                    s0 ^= self.x[0]
                    s1 ^= self.x[1]
                self.next()
        self.x = [s0,s1]


    #----------------------------------------------------------
    #  init
    #
    def init(self, seed=12345):
        """Initialize the generator"""

        self.x = [seed, (seed >> 4) ^ (seed+seed)]


    #----------------------------------------------------------
    #  __init__
    #
    def __init__(self, seed=12345):
        """Constructor"""

        self.init(seed)

