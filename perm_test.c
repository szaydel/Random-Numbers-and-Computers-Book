//
//  file: perm_test.c
//
//  Permutation test
//
//  RTK, 13-Oct-2017
//  Last update:  13-Oct-2017
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


int perm(int t, double *b) {
    int r=t,f=0,s,i;
    double m;

    while (r>1) {
        m=-1.0;
        for(i=0; i<r; i++) {
            if (b[i]>m) {
                m=b[i];
                s=i;
            }
        }
        f = r*f+s;
        m = b[s];
        b[s] = b[r-1];
        b[r-1] = m;
        r--;
    }
    return f;
}


int fact(int n) {
    if (n==1) return 1;
    else return n*fact(n-1);
}


double perm_test(int t, int typ, FILE *g) {
    int n=0,i,*bins,bn,f;
    double *b,x, v=0.0, p;

    bn = fact(t);
    p = 1.0/(double)bn;
    b = (double*)malloc(t*sizeof(double));
    bins = (int*)malloc(bn*sizeof(int));
    memset((void*)b, 0, t*sizeof(double));
    memset((void*)bins, 0, bn*sizeof(int));

    while (1) {
        for(i=0; i<t; i++) {
            x = next(typ,g);
            if (x==-1) break;
            b[i] = x;
        }
        if (x==-1) break;
        f = perm(t,b);
        if (f<0) printf("f<0\n");
        if (f>(bn-1)) printf("f>%d\n", bn-1);
        bins[f]++;
        n++;
    }

    for(i=0; i<bn; i++)
        v += ((bins[i]-n*p)*(bins[i]-n*p))/(n*p);

    return v;
}


int main(int argc, char *argv[]) {
    FILE *g;
    int typ,t;
    double v, p;

    if (argc == 1) {
        printf("\nperm_test <t> 0|1|2|3 <source>\n\n");
        printf("  <t>      - test size (min 2, max 5)\n");
        printf("  0|1|2|3  - 0=byte, 1=unsigned 32-bit, 2=unsigned 64-bit, 3=double\n");
        printf("  <source> - source binary file\n\n");
        return 0;
    }

    t = atoi(argv[1]);
    if (t < 2) t=2;
    if (t > 5) t=5;
    typ = atoi(argv[2]);
    g = fopen(argv[3],"r");
    v = perm_test(t, typ, g);
    p = chisq_pval(v, fact(t));
    printf("\npermutation size=%d, chisq  = %0.8f, p-value = %0.8f\n\n", t, v, p);
}


