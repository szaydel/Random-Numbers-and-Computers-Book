//
//  file:  test_binomial.c
//
//  Test program for binomially distributed random numbers.
//
//  $ gcc test_binomial.c binomial.c -o test_binomial -lm
//
//  RTK, 19-Sep-2017
//  Last update:  19-Sep-2017  
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "binomial.h"

FILE *g;
double next(void) {
    double b;
    fread((void *)&b, sizeof(double), 1, g);
    return b;
}


void main(int argc, char *argv[]) {
    FILE *f;
    int i,n,m,N,d;
    double p;

    if (argc==1) {
        printf("\n\ntest_binomial 0|1|2 <n> <p> <uniform> <output>\n\n");
        printf("  0|1|2     - 0=cumulative, 1=recycle coin, 2=table\n");
        printf("  <n>       - number of trials\n");
        printf("  <p>       - probability of success per trial\n");
        printf("  <uniform> - file of uniformly distributed doubles\n"); 
        printf("  <output>  - output text file of samples\n\n");
        exit(0);
    }

    m = atoi(argv[1]);
    n = atoi(argv[2]);
    sscanf(argv[3],"%lf",&p);

    uniform = &next;
    
    f = fopen(argv[5],"wb");
    g = fopen(argv[4],"rb");
    fseek(g, 0, SEEK_END);
    N = (int)(ftell(g)/sizeof(double));
    fseek(g, 0, SEEK_SET);

    for(i=0; i<N; i++) {
        switch (m) {
            case 0:  d = binomial_cumulative(n,p);   break;
            case 1:  d = binomial_recycle_coin(n,p); break;
            case 2:  d = binomial_table(n,p);        break;
            default: break;
        }
        fwrite((void*)&d, sizeof(int), 1, f);
    }

    fclose(f);
}

