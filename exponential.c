//
//  file:  exponential.c
//
//  Code to sample from an exponential distribution given
//  uniform U[0,1) inputs.
//
//  RTK, 26-Sep-2017
//  Last update:  26-Sep-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>

//  Assign to a uniform generator
double (*uniform)(void);

double exponential(double lambda) {
    return -log(uniform()) / lambda;
}

