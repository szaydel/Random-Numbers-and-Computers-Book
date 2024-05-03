//
//  file: serial_test.c
//
//  2D serial test - build 2D histogram of pairs and apply
//                   chi-square test
//
//  RTK, 11-Oct-2017
//  Last update:  11-Oct-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>

#define BINS 16

double igf(double s, double z) {
    double sc=(1.0/s), sum=1.0;
    double num=1.0, denom=1.0;
    int i;

    if (z < 0.0)
        return 0.0;

    sc *= pow(z,s);
    sc *= exp(-z);

    for(i=0; i<60; i++) {
        num *= z;
        s++;
        denom *= s;
        sum += (num/denom);
    }

    return sum*sc;
}


double chisq_pval(double cv, int dof) {
    double k = (double)dof * 0.5;
    double x = cv * 0.5;

    if ((cv < 0) || (dof < 1))
        return 0.0;
    if (dof == 2)
        return exp(-x);

    return igf(k,x)/tgamma(k);
}


int next(int typ, FILE *g) {
    uint8_t b8;
    uint32_t b32;
    uint64_t b64;
    double d;

    switch (typ) {
        case 0:
            if (fread((void*)&b8, sizeof(uint8_t), 1, g) == 0)
                return -1;
            return (int)(BINS*(b8/256.0));
            break;
        case 1:
            if (fread((void*)&b32, sizeof(uint32_t), 1, g) == 0)
                return -1;
            return (int)(BINS*(b32/4294967296.0));
            break;
        case 2:
            if (fread((void*)&b64, sizeof(uint64_t), 1, g) == 0)
                return -1;
            return (int)(BINS*((b64>>32)/4294967296.0));
            break;
        case 3:
            if (fread((void*)&d, sizeof(double), 1, g) == 0)
                return -1;
            return (int)(BINS*d);
            break;
        default:
            break;
    }
    return -1;
}


double chisq(int typ, FILE *g) {
    int n=0,i,j,b0,b1,bins[BINS*BINS];
    double v=0.0, p=1.0/(BINS*BINS);

    memset((void*)bins, 0, (BINS*BINS)*sizeof(int));

    while (1) {
        b0 = next(typ,g);
        if (b0 == -1) break;
        b1 = next(typ,g);
        if (b1 == -1) break;
        bins[BINS*b0+b1]++;
        n++;
    }

    for(i=0; i<(BINS*BINS); i++)
        v += ((bins[i]-n*p)*(bins[i]-n*p))/(n*p);

    return v;
}


int main(int argc, char *argv[]) {
    FILE *g;
    int typ;
    double v, p;

    if (argc == 1) {
        printf("\nserial_test 0|1|2|3 <source>\n\n");
        printf("  0|1|2|3  - 0=byte, 1=unsigned 32-bit, 2=unsigned 64-bit, 3=double\n");
        printf("  <source> - source binary file\n\n");
        return 0;
    }

    typ = atoi(argv[1]);
    g = fopen(argv[2],"r");
    v = chisq(typ, g);
    p = chisq_pval(v, BINS*BINS-1);
    printf("\nchisq  = %0.8f, p-value = %0.8f\n\n", v, p);
    fclose(g);
}


