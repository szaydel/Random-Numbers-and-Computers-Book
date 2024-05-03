#include <stdio.h>
#include <stdint.h>

void main(int argc, char *argv[]) {
    double r,x=0.1,y=0.1,xt,yt; 
    uint8_t b;
    uint32_t i,j,nsamp;
    FILE *f;

    if (argc == 1) {
        printf("\nlogistic_rng <r> <x0> <y0> <nsamp> <output>\n\n");
        printf("  <r>      - parameter, r > 3.56995\n");
        printf("  <x0>     - x0 initial value, (0,1)\n");
        printf("  <y0>     - y0 initial value, (0,1)\n");
        printf("  <nsamp>  - number of uint8 samples\n");
        printf("  <output> - output file name\n\n");
        return;
    }

    sscanf(argv[1],"%lf",&r);
    sscanf(argv[2],"%lf",&x);
    sscanf(argv[3],"%lf",&y);
    nsamp = (uint32_t)atol(argv[4]);
    f = fopen(argv[5],"w");

    for(i=0; i<1000; i++) {
        x = r*x*(1.0-x);
        y = r*y*(1.0-y);
    }

    for(i=0; i<nsamp; i++) {
        b = 0;
        for(j=0; j<8; j++) {
            xt = r*x*(1.0-x);
            yt = r*y*(1.0-y);
            x = yt;
            y = xt;
            b |= ((xt>=yt)<<j);
        }
        fwrite((void*)&b, sizeof(uint8_t), 1, f);
    }
    
    fclose(f);
}

