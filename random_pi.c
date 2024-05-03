//
//  file:  random_pi.c
//
//  Estimate pi using a file of random bytes
//
//  RTK, 15-Jul-2017
//  Last update:  15-Jul-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include <math.h>

int in_circle(FILE *f, int s) {
    uint64_t i, a=0, b=0;
    double x,y;

    //  Get the proper integer values
    for (i=0; i<s; i++) {
        a <<= 8;
        a |= (int)fgetc(f);
        b <<= 8;
        b |= (int)fgetc(f);
    }

    //  Convert to floating-point
    x = (double)a / (1ULL << (8*s));
    y = (double)b / (1ULL << (8*s));

    //  Is this point within the circle?
    return (sqrt(x*x+y*y) <= 1.0);
}


void main(int argc, char *argv[]) {
    FILE *f;
    int i,s,c=0,k=1000000;

    s = atoi(argv[1]);
    f = fopen(argv[2], "r");

    for (i=0; i < k; i++)
        c += in_circle(f,s);

    fclose(f);
    printf("Estimated value of pi: %0.7f\n\n", 4.0*((double)c/k));
}
