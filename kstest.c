//
//  file: kstest.c
//
//  Kolmogorov-Smirnov test.
//
//  RTK, 10-Oct-2017
//  Last update:  26-Oct-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>

#define EMAX 100
#define MAX_VALUES 1000

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


double kstest(int typ, FILE *g) {
    int i,k,n;
    size_t w = sizeof(uint8_t);
    double d,dp=-1.0,dm=-1.0,kp,km, z[EMAX], *p;

    //  Zero the EDF values
    memset((void*)z, 0, EMAX*sizeof(double));

    //  Find out how many values we need to store (< MAX_VALUES)
    if (typ==1) w = sizeof(uint32_t);
    if (typ==2) w = sizeof(uint64_t);
    if (typ==3) w = sizeof(double);
    fseek(g, 0, SEEK_END);
    n = ftell(g) / w;
    fseek(g, 0, SEEK_SET);
    if (n > MAX_VALUES) n = MAX_VALUES;

    //  Read the input file once
    p = (double *)malloc(n*sizeof(double));

    for(i=0; i<n; i++)
        p[i] = next(typ,g);

    //  Generate the empirical distribution using the deviations from uniform
    for(i=0; i<EMAX; i++)
        for(k=0; k<n; k++)
            if (p[k] <= ((double)i/EMAX)) z[i]++;
    for(i=0; i<EMAX; i++) {
        d = z[i]/n - (double)i/EMAX;
        if (d > 0)
            if (d > dp)
                dp = d;
        if (d < 0)
            if (fabs(d) > dm)
                dm = fabs(d);
    }

    kp = dp*sqrt(n);
    km = dm*sqrt(n);

    //  Free memory
    free(p);

    // 5% = 0.1548 < kp&km < 1.2186 = 95% - from Knuth Table 2 - n=1000
    printf("kp=%0.6f, km=%0.6f\n", kp,km);
}


int main(int argc, char *argv[]) {
    FILE *g;
    int typ;
    double v, p;

    if (argc == 1) {
        printf("\nkstest 0|1|2|3 <source>\n\n");
        printf("  0|1|2|3  - 0=byte, 1=unsigned 32-bit, 2=unsigned 64-bit, 3=double\n");
        printf("  <source> - source binary file\n\n");
        return 0;
    }

    typ = atoi(argv[1]);
    g = fopen(argv[2],"r");
    v = kstest(typ, g);
    //p = chisq_pval(v, 255);
    //printf("\nchisq  = %0.8f, p-value = %0.8f\n\n", v, p);
    fclose(g);
}


