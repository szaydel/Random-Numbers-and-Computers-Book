//
//  file: max_test.c
//
//  Maximum of t test
//
//  RTK, 12-Oct-2017
//  Last update:  12-Oct-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>

#define NBINS 20

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


double next(int typ, FILE *g) {
    uint8_t b8;
    uint32_t b32;
    uint64_t b64;
    double d;

    switch (typ) {
        case 1:
            if (fread((void*)&b32, sizeof(uint32_t), 1, g) == 0)
                return -1;
            return (double)b32/4294967296.0;
            break;
        case 2:
            if (fread((void*)&b64, sizeof(uint64_t), 1, g) == 0)
                return -1;
            return (double)(b64>>32)/4294967296.0;
            break;
        case 3:
            if (fread((void*)&d, sizeof(double), 1, g) == 0)
                return -1;
            return d;
            break;
        default:
            break;
    }
    return -1;
}


double max_of_t(int t, int typ, FILE *g) {
    int i,n=0,bins[NBINS];
    double b,m, v=0.0, p=1.0/(double)NBINS;

    memset((void*)bins, 0, NBINS*sizeof(int));

    while (1) {
        m = -1.0;
        for(i=0; i<t; i++) {
            b = next(typ,g);
            if (b == -1) break;
            if (b > m) m=b;
        }
        if (b == -1) break;
        n++;
        bins[(int)(NBINS*pow(m,t))]++;
    }

    for(i=0; i<NBINS; i++)
        v += ((bins[i]-n*p)*(bins[i]-n*p))/(n*p);

    return v;
}


int main(int argc, char *argv[]) {
    FILE *g;
    int typ, t;
    double v, p;

    if (argc == 1) {
        printf("\nmax_test <t> 0|1|2|3 <source>\n\n");
        printf("  <t>      - run length (e.g. 10)\n");
        printf("  1|2|3    - 1=unsigned 32-bit, 2=unsigned 64-bit, 3=double\n");
        printf("  <source> - source binary file\n\n");
        return 0;
    }

    t = atoi(argv[1]);
    typ = atoi(argv[2]);
    g = fopen(argv[3],"r");
    v = max_of_t(t, typ, g);
    p = chisq_pval(v, NBINS-1);
    printf("\nmax-of-%d chisq  = %0.8f, p-value = %0.8f\n\n", t, v, p);
    fclose(g);
}


