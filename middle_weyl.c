//
//  file: middle_weyl.c
//
//  Modified middle square PRNG
//  See: arxiv 1704.00358 by Widynski
//
//////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

uint64_t seed = 0xb5ad4eceda1ce2a9;

uint32_t random(void) {
    static uint64_t x=0, w=0;
    x *= x;
    w += seed;
    x += w;
    x = (x >> 32) | (x << 32);
    return (uint32_t)x;
}

void main() {
    int i;
    FILE *f;
    uint32_t n;
    double d;

    f = fopen("random_middle_weyl.dat","w");
    for(i=0; i<100000000; i++) {
        n = random();
        fwrite((void *)&n, 4, 1, f);
    }
    fclose(f);
    
    seed = 0xb5ad4eceda1ce2a9;
    f = fopen("double_middle_weyl.dat","w");
    for(i=0; i<100000000; i++) {
        d = (double)random()/4294967296.0;
        fwrite((void *)&d, sizeof(double), 1, f);
    }
    fclose(f);
}

