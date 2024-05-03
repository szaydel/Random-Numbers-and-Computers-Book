//
//  file:  kiss32.c
//
//  Marsaglia's KISS11, 32-bit version
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

uint32_t Q[4194304];    // MWC state
uint32_t carry=0;       // MWC carry
uint32_t cng=123456789; // LCG state
uint32_t xs=362436069;  // xorshift state

//  xorshift
uint32_t xorshift32(void) {
    xs^=(xs<<13);
    xs^=(xs>>17);
    xs^=(xs<<5);
    return xs;
}

//  LCG
uint32_t lcg32(void) {
    cng = 69069*cng + 13579;
    return cng;
}

//  multiply-with-carry
uint32_t mwc32(void) {
    uint32_t t,x;
    static int j=4194303;

    j=(j+1)&4194303;
    x=Q[j]; 
    t=(x<<28)+carry;
    carry=(x>>4)-(t<x);

    return (Q[j]=t-x);
}

//  KISS
void kiss32_init(void) {
    int i;
    for(i=0; i<4194304; i++) {
        Q[i] = lcg32() + xorshift32();
    }
}

uint32_t kiss32(void) {
    return mwc32() + xorshift32() + lcg32();
}

double dkiss32(void) {
    return (double)kiss32()/ 4294967297.0;
}


//  Test
void main(void) {
    uint32_t i,b;
    FILE *g;

    g = fopen("random_kiss32.dat", "w");

    kiss32_init();
    for(i=0; i<10000000; i++) {
        b = kiss32();
        fwrite((void*)&b, sizeof(uint32_t), 1, g);
    }
    fclose(g);
}

