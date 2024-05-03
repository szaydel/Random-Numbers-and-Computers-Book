#
#  file:  Func.py
#
#  Evaluate a function represented by a vector
#
#  RTK, 12-Dec-2017
#  Last update:  31-Dec-2017
#
###############################################################

from types import *

class Func:
    """Evaluate a vector as a function"""

    def reg(self,a):
        """Force in bounds as a register number"""
        return int(a) % 10


    def op(self,a):
        """Force in bounds as an operator number"""
        return int(a) % 11


    def val(self,a,b):
        """Return as an operand value"""

        if (b < 0):
            ans = "x"  # x
        elif (b > 0):
            ans = "%d" % self.reg(a)  # register number
        else:
            ans = int(abs(a)) & 0xffff
        return ans

    def r(self,n):
        """Keep in uint32 range"""
        if (n < 0):
            n += 65536
        return n & 0xffff

    def num(self,a):
        """Get the proper numeric value"""
        if (a=="x"):
            ans = self.x
        elif (type(a) == StringType):
            ans = self.m[int(a)]
        else:
            ans = a
        return ans

    def op_plus(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) + self.num(op1))
    def op_minus(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) - self.num(op1))
    def op_mult(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) * self.num(op1))
    def op_div(self,op0,op1,dst):
        self.m[dst] = self.r(int(self.num(op0) / (1e-9+self.num(op1))))
    def op_mod(self,op0,op1,dst):
        if (self.num(op1)==0):
            self.m[dst] = 0
        else:
            self.m[dst] = self.r(self.num(op0) % self.num(op1))
    def op_or(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) | self.num(op1))
    def op_and(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) & self.num(op1))
    def op_xor(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) ^ self.num(op1))
    def op_set(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0))
    def op_gt(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) > self.num(op1))
    def op_lt(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) < self.num(op1))
    def op_eq(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) == self.num(op1))
    def op_ne(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) != self.num(op1))
    def op_ge(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) >= self.num(op1))
    def op_le(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) <= self.num(op1))
    def op_sup(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) << (self.num(op1) % 16))
    def op_sdown(self,op0,op1,dst):
        self.m[dst] = self.r(self.num(op0) >> (self.num(op1) % 16))
    
    def Eval(self):
        """Evaluate"""

        self.m = [0]*10
        i=0
        while (i < len(self.v)):
            op  = self.op(self.v[i])
            op0 = self.val(self.v[i+1], self.v[i+2])
            op1 = self.val(self.v[i+3], self.v[i+4])
            dst = self.reg(self.v[i+5])
            i += 6
            self.t[op](op0,op1,dst)
        
        return self.m[self.reg(self.v[-1])]

    def pp_op(self,op):
        return ["+","-","*","/","%","|","&",
                "^","set","<<",">>"][op]

    def pp_fmt(self,a,b):
        if (b < 0):
            ans = "x"
        elif (b > 0):
            ans = "r%d" % self.reg(a)
        else:
            ans = str(int(abs(a)) & 0xffff)
        return ans

    def __repr__(self):
        """Pretty print"""

        s=""
        i=0
        while (i < len(self.v)):
            op  = self.op(self.v[i])
            op0 = self.pp_fmt(self.v[i+1], self.v[i+2])
            op1 = self.pp_fmt(self.v[i+3], self.v[i+4])
            dst = self.reg(self.v[i+5])
            i += 6
            if (op==9) or (op==10):
                s += "r%d = %s %s (%s %% 16)\n" % (dst, op0, self.pp_op(op), op1)
            elif (op!=8):
                s += "r%d = %s %s %s\n" % (dst, op0, self.pp_op(op), op1)
            else:
                s += "r%d = %s\n" % (dst, op0)
        
        return s


    def __str__(self):
        return self.__repr__()

    def asCode(self, name="gp_rand"):
        """Output as a C function"""

        s  = "uint16_t %s(uint16_t x) {\n" % name
        s += "    uint16_t r0=0,r1=0,r2=0,r3=0,r4=0;\n"
        s += "    uint16_t r5=0,r6=0,r7=0,r8=0,r9=0;\n\n"
        for t in self.__repr__().split("\n"):
            if (t != ""):
                s += "    " + t + ";\n"
        s += "    return r%d;\n" % self.v[-1]
        s += "}\n"
        return s


    def __init__(self, v, x):
        """Constructor"""

        self.v = v
        self.x = x

        self.t = [
            self.op_plus,
            self.op_minus,
            self.op_mult,
            self.op_div,
            self.op_mod,
            self.op_or,
            self.op_and,
            self.op_xor,
            self.op_set,
            self.op_sup,
            self.op_sdown,
        ]

