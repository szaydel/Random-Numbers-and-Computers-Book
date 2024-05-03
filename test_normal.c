//
//  file:  test_normal.c
//
//  Test program for normally distributed random numbers.
//
//  $ gcc test_normal.c normal.c -o test_normal -lm
//
//  RTK, 14-Sep-2017
//  Last update:  16-Sep-2017  
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "normal.h"

FILE *g;
double next(void) {
    double b;
    fread((void *)&b, sizeof(double), 1, g);
    return b;
}


void main(int argc, char *argv[]) {
    FILE *f;
    int i,n;
    double d;

    if (argc==1) {
        printf("\ntest_normal 0|1 <uniform> <output>\n");
        printf("  0|1       - 0=Box-Muller, 1=polar\n");
        printf("  <uniform> - file of uniformly distributed doubles\n");
        printf("  <output>  - output file for normally distributed doubles\n");
        printf("\n");
        exit(0);
    }

    uniform = &next;
    
    f = fopen(argv[3], "wb");
    g = fopen(argv[2],"rb");
    fseek(g, 0, SEEK_END);
    n = (int)(0.7*(ftell(g)/sizeof(double)));
    fseek(g, 0, SEEK_SET);

    if (atoi(argv[1]) == 0) {
        for(i=0; i<n; i++) {
            d = norm_box();
            fwrite((void *)&d, sizeof(double), 1, f);
        }
    } else {
        for(i=0; i<n; i++) {
            d = norm_polar();
            fwrite((void *)&d, sizeof(double), 1, f);
        }
    }

    fclose(f);
}

