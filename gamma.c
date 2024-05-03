//
//  file:  gamma.c
//
//  Code to sample from a gamma distribution given
//  uniform U[0,1) inputs.
//
//  RTK, 24-Sep-2017
//  Last update:  24-Sep-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include <math.h>

//  Assign to a uniform generator
double (*uniform)(void);

//  Ahrens GC algorithm - standard gamma
#define DPI 3.1415926535897932

double gc_gamma(double a) {
    double b,c,s,x,f,t,u;

    b = a - 1.0;
    c = a+b;
    s = sqrt(c);
    while (1) {
        while (1) {
            t = s*tan(DPI*(uniform()-0.5));
            x = b + t;
            if (x >= 0.0) break;
        }
        u = uniform();
        f = exp(b*log(x/b) - t + log(1.0 + t*t/c));
        if (u <= f) return x;
    }
}

//  Two-parameter gamma
double gamma_gc(double a, double b) {
    return (a > 1.0) ? gc_gamma(a)/b
           : gc_gamma(a+1.0)*pow(uniform(),1.0/a)/b;
}


//
//  Marsaglia and Tsang - standard gamma
//

//  Box-Muller transform - N(0,1)
#define TPI 6.283185307179586231996

double norm_box(void) {
    static uint8_t f=0;
    double m,u1,u2,z1;
    static double z2;

    if (f) {
        f = 0;
        return z2;
    } else {
        u1 = uniform();
        u2 = uniform();
        m = sqrt(-2.0*log(u1));
        z1 = m*cos(TPI*u2);
        z2 = m*sin(TPI*u2);
        f = 1;
        return z1;
    }
}

double ms_gamma(double a) {
    double d,c,x,v,u;

    d = a - 1.0/3.0;
    c = 1.0 / sqrt(9.0*d);

    while (1) {
        do {
            x = norm_box();
            v = 1.0 + c*x;
        } while (v <= 0.0);
        v = v*v*v;
        u = uniform();
        if (u < 1.0-0.0331*(x*x)*(x*x)) return d*v;
        if (log(u) < 0.5*x*x+d*(1.0-v+log(v))) return d*v;
    }
}
    

//  Two-parameter gamma
double gamma_ms(double a, double b) {
    return (a > 1.0) ? ms_gamma(a)/b
           : ms_gamma(a+1.0)*pow(uniform(),1.0/a)/b;
}


//  chi-square
double chisq(double k) {
    return gamma_gc(k/2.0, 0.5);
}

