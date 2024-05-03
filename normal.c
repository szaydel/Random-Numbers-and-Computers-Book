//
//  file:  normal.c
//
//  Code to sample from a normal distribution given
//  uniform U[0,1) inputs.
//
//  RTK, 14-Sep-2017
//  Last update:  14-Sep-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include <math.h>

//  Assign to a uniform generator
double (*uniform)(void);

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


//  Marsaglia polar transform
double norm_polar(void) {
    static uint8_t f=0;
    double m,s,u1,u2,z1;
    static double z2;

    if (f) {
        f = 0;
        return z2;
    } else {
        do {
            u1 = 2*uniform()-1;
            u2 = 2*uniform()-1;
            s = u1*u1 + u2*u2;
        } while ((s >= 1) || (s == 0));
        m = sqrt(-2.0*log(s)/s);
        z1 = u1*m;
        z2 = u2*m;
        f = 1;
        return z1;
    }
}

