#
#  file:  APFP.py
#
#  Simple arbitrary-precision fixed-point routines
#
#  RTK, 07-Jul-2016
#  Last update:  09-Jul-2016
#
###############################################################

from types import *


###############################################################
#  APFP
#
class APFP:
    """Simple base-10 arbitrary-precision fixed-point routines"""

    #
    #  Set this once before generating values, treated as global for simplicity
    #
    dprec = 23  #  scale factor: decimal digits


#
#  Private:
#

    #----------------------------------------------------------
    #  __toFP
    #
    def __toFP(self, arg):
        """Represent the string argument as scaled integer"""

        sign = 1
        arg = arg.strip()
        if (arg[0] == "."):
            arg = "0" + arg
        if (arg[0] == "-"):
            sign = -1
            arg = arg[1:]
        if (arg[0] == "+"):
            arg = arg[1:]
        idx = arg.find(".")
        ipart = arg[0:idx]
        fpart = arg[(idx+1):]
        flen = len(fpart)
        rounding = 0

        if (flen <= APFP.dprec):
            #  Everything fits - add trailing zeros
            fpart += "0"*(APFP.dprec-flen)
        else:
            #  Need to truncate w/rounding
            if (int(fpart[APFP.dprec]) >= 5):
                rounding = 1
            fpart = fpart[0:APFP.dprec]
        
        self.n = sign*(int(ipart) * 10**APFP.dprec + int(fpart) + rounding)


#
#  Public:
#

    #----------------------------------------------------------
    #  __str__
    #
    def __str__(self):
        """Return a string representation"""

        if (self.n == 0):
            return "0.0"

        s = str(abs(self.n))
        if (len(s) <= APFP.dprec):
            s = "0"*(APFP.dprec-len(s)) + s
            ans = "0." + s
        else:
            ip = len(s) - APFP.dprec
            ans = s[0:ip] + "." + s[ip:]
        
        k = len(ans)-1
        while (ans[k] == "0"):
            k -= 1
        ans = ans[0:(k+1)]
        if (ans[-1] == "."):
            ans += "0"

        if (self.n < 0):
            ans = "-" + ans

        return ans


    #----------------------------------------------------------
    #  __repr__
    #
    def __repr__(self):
        return self.__str__()


    #----------------------------------------------------------
    #  __pow__
    #
    def __pow__(self, n):
        """Number raised to an integer power"""

        z = APFP(1)

        for i in xrange(n):
            z = self * z 

        return z


    #----------------------------------------------------------
    #  __abs__
    #
    def __abs__(self):
        """Return a positive APFP number"""

        z = APFP()
        z.n = abs(self.n)
        return z


    #----------------------------------------------------------
    #  __float__
    #
    def __float__(self):
        return float(self.__str__())


    #----------------------------------------------------------
    #  __int__
    #
    def __int__(self):
        ans = self.__str__()
        idx = ans.find(".")
        return int(ans[0:idx])


    #----------------------------------------------------------
    #  __lt__
    #
    def __lt__(self, b):
        """Is number < b?"""

        return self.n < b.n
                

    #----------------------------------------------------------
    #  __gt__
    #
    def __gt__(self, b):
        """Is number > b?"""

        return self.n > b.n
                

    #----------------------------------------------------------
    #  __le__
    #
    def __le__(self, b):
        """Is number <= b?"""

        return self.n <= b.n
                

    #----------------------------------------------------------
    #  __ge__
    #
    def __ge__(self, b):
        """Is number >= b?"""

        return self.n >= b.n


    #----------------------------------------------------------
    #  __eq__
    #
    def __eq__(self, b):
        """Is number == b?"""

        return self.n == b.n


    #----------------------------------------------------------
    #  __ne__
    #
    def __ne__(self, b):
        """Is number != b?"""

        return self.n!= b.n


    #----------------------------------------------------------
    #  __add__
    #
    def __add__(self, b):
        """Add two APFP numbers"""

        z = APFP()
        z.n = self.n + b.n
        return z


    #----------------------------------------------------------
    #  __neg__
    #
    def __neg__(self):
        z = APFP()
        z.n = -self.n
        return z


    #----------------------------------------------------------
    #  __sub__
    #
    def __sub__(self, b):
        """Subtract two APFP numbers"""

        z = APFP()
        z.n = self.n - b.n
        return z


    #----------------------------------------------------------
    #  __mul__
    #
    def __mul__(self, b):
        """Multiply by b"""

        z = APFP()
        z.n = (self.n * b.n + 5*10**(APFP.dprec-1)) / 10**APFP.dprec
        return z


    #----------------------------------------------------------
    #  __div__
    #
    def __div__(self, b):
        """Divide by b"""

        z = APFP()
        z.n = (self.n * 10**APFP.dprec + (5*b.n) / 10) / b.n
        return z

    
    #
    #  Using series summation (adapted from Python decimal module documentation):
    #

    #----------------------------------------------------------
    #  cos
    #
    def cos(self):
        """Cosine"""

        i, lasts, s, fact, num, sign = [APFP(j) for j in [0,0,1,1,1,1]]
        self.terms = 0

        while (s != lasts):
            lasts = s
            i += APFP("2")
            fact *= i * (i - APFP("1"))
            num *= self * self
            sign *= APFP("-1")
            s += num / fact * sign
            self.terms += 1

        return s


    #----------------------------------------------------------
    #  sin
    #
    def sin(self):
        """Sine"""

        i, lasts, fact, sign = [APFP(j) for j in [1,0,1,1]]
        s = self
        num = self
        self.terms = 0

        while (s != lasts):
            lasts = s
            i += APFP("2")
            fact *= i * (i - APFP("1"))
            num *= self * self
            sign *= APFP("-1")
            s += num / fact * sign
            self.terms += 1

        return s


    #----------------------------------------------------------
    #  tan
    #
    def tan(self):
        """Tangent"""

        return self.sin() / self.cos()


    #----------------------------------------------------------
    #  exp
    #
    def exp(self):
        """Exponential"""

        i, lasts, s, fact, num = [APFP(j) for j in [0,0,1,1,1]]
        self.terms = 0

        while s != lasts:
            lasts = s
            i += APFP("1")
            fact *= i
            num *= self
            s += num / fact
            self.terms += 1

        return s


    #
    #  Using Newton's method (based on fixed.c versions):
    #

    #----------------------------------------------------------
    #  sqrt
    #
    def sqrt(self):
        """Square root"""

        if (self < APFP(0)):
            return APFP(-1)
        if (self == APFP(1)):
            return APFP(1)

        s = self / APFP(2)
        lasts = APFP(0)
        self.iters = 0

        while (s != lasts):
            lasts = s
            s = APFP("0.5") * (s + self / s)
            self.iters += 1
        
        return s


    #----------------------------------------------------------
    #  log
    #
    def log(self, imax=20):
        """Natural logarithm"""

        if (self <= APFP(0)):
            return APFP(-1)
        if (self == APFP(1)):
            return APFP(0)

        s = self / APFP(10)
        lasts = APFP(0)
        self.iters = 0
        i = 0

        while (s != lasts) and (i < imax):
            lasts = s
            s += self * (-s).exp()
            s -= APFP(1)
            i += 1
            self.iters += 1

        return s


    #----------------------------------------------------------
    #  __init__
    #
    def __init__(self, x="0"):
        """Represent x"""

        self.terms = 0
        self.iters = 0

        if (type(x) == StringType) or (type(x) == UnicodeType):
            if (x.find(".") != -1):
                self.__toFP(str(x))
            else:
                self.__toFP(str(x)+".0")  # ensure .
        elif (type(x) == FloatType):
            self.__toFP("%0.18f" % x)  # enough precision for double
        elif (type(x) == IntType) or (type(x) == LongType):
            self.__toFP("%d.0" % x)
        else:
            raise ValueError("unsupported value")


#  end APFP.py

