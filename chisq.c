//
//  file: chisq.c
//
//  Test a sequence calculating the chi-square p-value.
//  Really the "frequency test" for 256 bins.
//
//  RTK, 10-Oct-2017
//  Last update:  10-Oct-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>

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
            return (int)b8;
            break;
        case 1:
            if (fread((void*)&b32, sizeof(uint32_t), 1, g) == 0)
                return -1;
            return (int)(b32>>24);
            break;
        case 2:
            if (fread((void*)&b64, sizeof(uint64_t), 1, g) == 0)
                return -1;
            return (int)(b64>>56);
            break;
        case 3:
            if (fread((void*)&d, sizeof(double), 1, g) == 0)
                return -1;
            return (int)(256*d);
            break;
        default:
            break;
    }
    return -1;
}


double chisq(int typ, FILE *g) {
    int n=0,i,b,bins[256];
    double v=0.0, p=1.0/256.0;

    memset((void*)bins, 0, 256*sizeof(int));

    while (1) {
        b = next(typ,g);
        if (b == -1) break;
        bins[b]++;
        n++;
    }

    for(i=0; i<256; i++)
        v += ((bins[i]-n*p)*(bins[i]-n*p))/(n*p);

    return v;
}


int main(int argc, char *argv[]) {
    FILE *g;
    int typ;
    double v, p;

    if (argc == 1) {
        printf("\nchisq 0|1|2|3 <source>\n\n");
        printf("  0|1|2|3  - 0=byte, 1=unsigned 32-bit, 2=unsigned 64-bit, 3=double\n");
        printf("  <source> - source binary file\n\n");
        return 0;
    }

    typ = atoi(argv[1]);
    g = fopen(argv[2],"r");
    v = chisq(typ, g);
    p = chisq_pval(v, 255);
    printf("\nchisq  = %0.8f, p-value = %0.8f\n\n", v, p);
    fclose(g);
}


