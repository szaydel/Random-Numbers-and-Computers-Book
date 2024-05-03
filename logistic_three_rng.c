#include <stdio.h>
#include <stdint.h>

void main(int argc, char *argv[]) {
    double r=3.9999,x=0.2,y=0.3,xt,yt; 
    uint32_t i,j,nsamp,u0,u1;
    FILE *f;

    if (argc == 1) {
        printf("\nlogistic_three_rng <r> <x0> <y0> <nsamp> <output>\n\n");
        printf("  <r>      - parameter, [3.9,4.0)\n");
        printf("  <x0>     - x0 initial value, (0,1)\n");
        printf("  <y0>     - y0 initial value, (0,1)\n");
        printf("  <nsamp>  - number of uint32 samples\n");
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
        xt = r*x*(1.0-x);
        yt = r*y*(1.0-y);
        x = yt;
        y = xt;
        u0 = (uint32_t)(0xFFFFFFFF*xt);
        u1 = (uint32_t)(0xFFFFFFFF*yt);
        u1 = (u1>>24) | (u1<<24) | ((u1>>16)&0xff)<<8 | ((u1>>8)&0xff)<<16;
        u0 ^= u1;
        fwrite((void*)&u0, sizeof(uint32_t), 1, f);
    }
    
    fclose(f);
}

