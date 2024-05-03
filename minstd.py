#
#  file:  minstd.py
#
#  MINSTD in Python
#
#  RTK, 13-Nov-2017
#  Last update:  03-Dec-2017
#
###############################################################

###############################################################
#  MINSTD
#
class MINSTD:
    """Encapsulate MINSTD (any multiplicative LCG if A and M given)"""

    #----------------------------------------------------------
    #  next
    #
    def next(self):
        """Next random value"""

        self.x = (self.A*self.x) % self.M
        return self.x

    
    #----------------------------------------------------------
    #  next_fp
    #
    def next_fp(self):
        """Next random value as a float"""

        return float(self.next()) * 4.656612875245797e-10

    
    #----------------------------------------------------------
    #  jump
    #
    def jump(self, p):
        """Jump ahead p values"""

        def mod_pow(base, exponent, modulus):
            if modulus == 1:
                return 0 
            c = 1
            for i in xrange(1, exponent+1): 
                c = (c * base) % modulus
            return c

        ap = mod_pow(self.A, p, self.M)
        self.x = (ap * self.x) % self.M


    #----------------------------------------------------------
    #  init
    #
    def init(self, seed=12345):
        """Initialize the generator"""

        self.x = seed


    #----------------------------------------------------------
    #  __init__
    #
    def __init__(self, seed=12345, A=48271, M=2147483647):
        """Constructor"""

        self.A = A
        self.M = M
        self.init(seed)

