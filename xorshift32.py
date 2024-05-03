#
#  file:  xorshift32.py
#
#  xorshift32 in Python with alternate parameters
#
#  RTK, 14-Nov-2017
#  Last update:  14-Nov-2017
#
###############################################################

import random

#  Possible parameters, select with constructor "stream" input
xorshift32_PARAMS = [
    [ 1, 3,10],[ 1, 5,16],[ 1, 5,19],[ 1, 9,29],[ 1,11, 6],[ 1,11,16],[ 1,19, 3],[ 1,21,20],[ 1,27,27],
    [ 2, 5,15],[ 2, 5,21],[ 2, 7, 7],[ 2, 7, 9],[ 2, 7,25],[ 2, 9,15],[ 2,15,17],[ 2,15,25],[ 2,21, 9],
    [ 3, 1,14],[ 3, 3,26],[ 3, 3,28],[ 3, 3,29],[ 3, 5,20],[ 3, 5,22],[ 3, 5,25],[ 3, 7,29],[ 3,13, 7],
    [ 3,23,25],[ 3,25,24],[ 3,27,11],[ 4, 3,17],[ 4, 3,27],[ 4, 5,15],[ 5, 3,21],[ 5, 7,22],[ 5, 9,7 ],
    [ 5, 9,28],[ 5, 9,31],[ 5,13, 6],[ 5,15,17],[ 5,17,13],[ 5,21,12],[ 5,27, 8],[ 5,27,21],[ 5,27,25],
    [ 5,27,28],[ 6, 1,11],[ 6, 3,17],[ 6,17, 9],[ 6,21, 7],[ 6,21,13],[ 7, 1, 9],[ 7, 1,18],[ 7, 1,25],
    [ 7,13,25],[ 7,17,21],[ 7,25,12],[ 7,25,20],[ 8, 7,23],[ 8,9,23 ],[ 9, 5,1 ],[ 9, 5,25],[ 9,11,19],
    [ 9,21,16],[10, 9,21],[10, 9,25],[11, 7,12],[11, 7,16],[11,17,13],[11,21,13],[12, 9,23],[13, 3,17],
    [13, 3,27],[13, 5,19],[13,17,15],[14, 1,15],[14,13,15],[15, 1,29],[17,15,20],[17,15,23],[17,15,26]]


###############################################################
#  xorshift32
#
class xorshift32:
    """Encapsulate xorshift32 with parameter choices"""


    #----------------------------------------------------------
    #  next
    #
    def next(self):
        """Next 32-bit value"""

        MASK = 0xffffffff
        self.x ^= (self.x << self.A) & MASK
        self.x ^= (self.x >> self.B) & MASK
        self.x ^= (self.x << self.C) & MASK
        return self.x


    #----------------------------------------------------------
    #  next_fp
    #
    def next_fp(self):
        """Next floating-point value"""

        return float(self.next()) * 2.3283064365386963e-10


    #----------------------------------------------------------
    #  init
    #
    def init(self, seed=12345):
        """Seed the generator"""

        self.x = seed


    #----------------------------------------------------------
    #  __init__
    #
    def __init__(self, seed=12345, stream=0):
        """Constructor"""
        
        global xorshift32_PARAMS

        self.stream = stream
        self.A, self.B, self.C = xorshift32_PARAMS[stream % len(xorshift32_PARAMS)]
        self.init(seed)


#  Scramble the parameters on load
random.shuffle(xorshift32_PARAMS)


