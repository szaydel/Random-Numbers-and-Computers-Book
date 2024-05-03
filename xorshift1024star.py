#
#  file:  xorshift1024star.py
#
#  xorshift1024* in Python 
#    (based on xorshift1024star.c by Sebastiano Vigna)
#
#  RTK, 13-Nov-2017
#  Last update:  14-Nov-2017
#
###############################################################

import struct

###############################################################
#  xorshift1024star
#
class xorshift1024star:
    """Encapsulate xorshift1024*"""

    #----------------------------------------------------------
    #  next
    #
    def next(self):
        """Next random 64-bit value"""

        MASK = 0xffffffffffffffff
        s0 = self.x[self.p]
        self.p = (self.p+1) & 15
        s1 = self.x[self.p]
        s1 ^= (s1 <<31) & MASK
        self.x[self.p] =   \
            (s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30)) & MASK
        res = self.x[self.p]
        res = (res * 1181783497276652981) & MASK
        return res

    
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

        JUMP = [
            0x84242f96eca9c41d,0xa3c65b8776f96855,0x5b34a39f070b5837,
            0x4489affce4f31a1e,0x2ffeeb0a48316f40,0xdc2d9891fe68c022,
            0x3659132bb12fea70,0xaac17d8efa43cab8,0xc4cb815590989b13,
            0x5ee975283d71c93b,0x691548c86c1bd540,0x7910c41d10a1e6a5,
            0x0b5fc64563b3e2a8,0x047f7684e9fc949d,0xb99181f2d8f685ca,
            0x284600e3f30e38c3]

        t = [0]*16
        for i in range(2):
            for b in range(64):
                if (JUMP[i] & (1<<b)):
                    for j in range(16):
                        t[j] ^= self.x[(j+self.p) & 15]
                self.next()
        for j in range(16):
            self.x[(j+self.p) & 15] = t[j]


    #----------------------------------------------------------
    #  init
    #
    def init(self, seed=12345):
        """Initialize the generator"""

        MASK = 0xffffffffffffffff
        self.x = [0]*16
        self.p = 0
        self.x[0] = seed & MASK
        for i in range(1,16):
            self.x[i] = (self.x[i-1]*seed + seed) & MASK


    #----------------------------------------------------------
    #  __init__
    #
    def __init__(self, seed=12345):
        """Constructor"""

        self.init(seed)


