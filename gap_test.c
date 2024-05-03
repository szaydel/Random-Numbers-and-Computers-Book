//
//  file: gap_test.c
//
//  Do Knuth's gap test for runs above and below the mean.
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

#define T 32

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
        case 0:
            if (fread((void*)&b8, sizeof(uint8_t), 1, g) == 0)
                return -1;
            return (double)b8/256.0;
            break;
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


double gap_test(double a, double b, int typ, FILE *g, double *v) {
    int n=100000000,s=0,i,r,count[T];
    double p=b-a, u, prob[T];

    memset((void*)count, 0, T*sizeof(int));

    //  Find gaps
    while (s < n) {
        r = 0;
        u = next(typ,g);
        if (u == -1) break;
        while ((u < a) || (u > b)) {
            r++;
            u = next(typ,g);
            if (u == -1) break;
        }
        if (u == -1) break;
        if (r >= T) count[T]+=1; else count[r]+=1;
        s++;
    }

    //  Set up the probabilities
    for(i=0; i<T; i++)
        prob[i] = p*pow(1.0-p, i);
    prob[0] = p;
    prob[T-1] = pow(1.0-p, T);

    //  Calculate chi-square (in v) and p-value
    *v = 0;
    for(i=0; i<T; i++)
        *v += (count[i]-s*prob[i])*(count[i]-s*prob[i])/(s*prob[i]);

    return chisq_pval(*v,T-1);
}


int main(int argc, char *argv[]) {
    FILE *g;
    int typ;
    double v, p;

    if (argc == 1) {
        printf("\ngap 0|1|2|3 <source>\n\n");
        printf("  0|1|2|3  - 0=byte, 1=unsigned 32-bit, 2=unsigned 64-bit, 3=double\n");
        printf("  <source> - source binary file\n\n");
        return 0;
    }

    typ = atoi(argv[1]);
    g = fopen(argv[2],"r");
    p = gap_test(0.0, 0.5, typ, g, &v);
    printf("\nruns below the mean, chisq  = %0.8f, p-value = %0.8f\n", v, p);
    fclose(g);
    g = fopen(argv[2],"r");
    p = gap_test(0.5, 1.0, typ, g, &v);
    printf("runs above the mean, chisq  = %0.8f, p-value = %0.8f\n\n", v, p);
    fclose(g);
}


