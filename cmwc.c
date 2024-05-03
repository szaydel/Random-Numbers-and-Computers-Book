//
//  file:  cmwc.c
//
//  Complimentary [sic] Multiply-With-Carry Generator
//

#include <stdio.h>
#include <inttypes.h>

//  xorshift to initialize CMWC
uint32_t s = 1;

uint32_t xorshift(void) {
    s ^= (s<<13);
    s ^= (s>>17);
    s ^= (s<< 5);
    return s;
}

//
//  CMWC4096
//
#define R 4096
#define A 18782ULL

uint32_t Q[R];
uint32_t c=362436; // c < 809430660

void CMWC_init(uint32_t seed) {
    int i;
    s = seed;
    for(i=0; i<R; i++)
        Q[i] = xorshift();
}

uint32_t CMWC(void) {
    static uint32_t i=R-1;
    uint64_t t;
    uint32_t x, m=0xFFFFFFFE;

    i = (i+1) & (R-1);
    t = A*Q[i] + c;
    c = t >> 32;
    x = t+c;
    if (x < c) {
        x++; c++;
    }
    Q[i] = m-x;
    return Q[i];
}


void main() {
    FILE *g;
    uint32_t i,b;

    CMWC_init(1);

    g = fopen("random_cmwc.dat","w");

    for(i=0; i<10000000; i++) {
        b = CMWC();
        fwrite((void *)&b, sizeof(uint32_t), 1, g);
    }

    fclose(g);
}

