#include <stdio.h>
#include <stdint.h>

void main(int argc, char *argv[]) {
    double r,x=0.1; 
    uint32_t i,nsamp,u;
    FILE *f;

    if (argc == 1) {
        printf("\nlogistic_rng <r> <nsamp> <output>\n\n");
        printf("  <r>      - parameter, r > 3.56995\n");
        printf("  <nsamp>  - number of uint32 samples\n");
        printf("  <output> - output file name\n\n");
        return;
    }

    sscanf(argv[1],"%lf",&r);
    nsamp = (uint32_t)atol(argv[2]);
    f = fopen(argv[3],"w");

    for(i=0; i<1000; i++)
        x = r*x*(1.0-x);

    for(i=0; i<nsamp; i++) {
        x = r*x*(1.0-x);
        u = (uint32_t)(0xffffffff*x);
        fwrite((void*)&u, sizeof(uint32_t), 1, f);
    }
    
    fclose(f);
}

