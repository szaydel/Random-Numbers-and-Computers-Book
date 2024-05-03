//
//  file:  kiss64.c
//
//  Marsaglia's KISS11, 64-bit version
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

uint64_t Q[2097152];                // MWC state
uint64_t carry=0;                   // MWC carry
uint64_t cng=123456789987654321LL;  // LCG state
uint64_t xs=362436069362436069LL;   // xorshift state

//  xorshift
uint64_t xorshift64(void) {
    xs^=(xs<<13);
    xs^=(xs>>17);
    xs^=(xs<<43);
    return xs;
}

//  LCG
uint64_t lcg64(void) {
    cng = 6906969069LL*cng + 13579;
    return cng;
}

//  multiply-with-carry
uint64_t mwc64(void) {
    uint64_t t,x;
    static int j=2097151;
    j=(j+1)&2097151;
    x=Q[j]; 
    t=(x<<28)+carry;
    carry=(x>>36)-(t<x);
    return (Q[j]=t-x);
}

//  KISS
void kiss64_init(void) {
    int i;
    for(i=0; i<2097152; i++) {
        Q[i] = lcg64() + xorshift64();
    }
}

uint64_t kiss64(void) {
    return mwc64() + xorshift64() + lcg64();
}

double dkiss64(void) {
    uint64_t n = (kiss64() >> 12) | 0x3FF0000000000000;
    return *(double *)&n - 1.0;
}


//  Test
void main(void) {
    uint64_t i,b;
    FILE *g;

    g = fopen("random_kiss64.dat", "w");

    kiss64_init();
    for(i=0; i<10000000; i++) {
        b = kiss64();
        fwrite((void*)&b, sizeof(uint64_t), 1, g);
    }
    fclose(g);
}

