//
//  file:  xorshift.c
//
//  Based on original version given in Marsaglia's paper for
//  periods 2^32-1 and 2^128-1.
//
//  Seeds can be anything but zero.
//

#include <stdio.h>

#define A 13
#define B 17
#define C  5

//                       y
unsigned long s32 = 2463534242UL;

unsigned long xorshift32(void) {
    s32 ^= (s32<<A);
    s32 ^= (s32>>B);
    s32 ^= (s32<<C);
    return s32;
}

// unsigned long xor(){
// static unsigned long y=2463534242;
// yˆ=(y<<13); y=(y>>17); return (yˆ=(y<<5)); } // typo in paper, should be y^=(y>>17)


// paper constants:          x         y         z        w
unsigned long s128[] = {123456789,362436069,521288629,88675123};

#define A128 11
#define B128  8
#define C128 19

unsigned long xorshift128(void) {
    unsigned long t;
    t = (s128[0]^(s128[0]<<A128));
    s128[0]=s128[1]; s128[1]=s128[2]; s128[2]=s128[3];
    s128[3] = (s128[3] ^ (s128[3]>>C128)) ^ (t^(t>>B128));
    return s128[3];
}

// unsigned long xor128(){
// static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
// unsigned long t;
// t=(xˆ(x<<11));x=y;y=z;z=w; return( w=(wˆ(w>>19))ˆ(tˆ(t>>8)) );

void main() {
    FILE *f;
    unsigned long b, i;

    f = fopen("random_xorshift128.dat","w");
    for(i=0; i < 10000000; i++) {
        b = xorshift128();
        fwrite((void*)&b, sizeof(unsigned long), 1, f);
    }
    fclose(f);

    f = fopen("random_xorshift32.dat","w");
    for(i=0; i < 10000000; i++) {
        b = xorshift32();
        fwrite((void*)&b, sizeof(unsigned long), 1, f);
    }
    fclose(f);
}

