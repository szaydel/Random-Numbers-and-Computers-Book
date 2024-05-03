//
//  file:  test_chacha20.c
//
//  compile with:
//      gcc test_chacha20.c -o test_chacha20 -O3 -ltomcrypt
//
//  RTK, 17-Jan-2018
//  Last update:  18-Jan-2018
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "tomcrypt.h"

#define N 1000000

void main(int argc, char *argv[]) {
    prng_state st;
    uint32_t n,i;
    uint8_t *out;
    FILE *g;
//    uint8_t ent[64] = { 12, 53, 75, 63,  4, 57,  8, 78, 89, 67,  8, 45, 62,  3, 41,  2,
//                         4, 56, 56, 67, 97, 88,  9, 78,  5, 67, 45, 23, 12, 43,  4, 76,
//                        74,  3,  8,  5, 34, 23,134, 56,  6,209, 45,233,  2, 42, 33,  5,
//                        67, 84, 98,209, 41, 23, 78,187, 36, 49, 58, 75, 67, 59, 83, 57};
//
    uint8_t ent[64] = {
        0xce,0x4e,0xad,0xb5,0x1a,0x7c,0x03,0xd1,0x10,0xd0,0x2b,0x42,0xdf,0xa3,0x72,0xb6,
        0x95,0xe1,0x3f,0xfa,0xc7,0xb8,0xd1,0xa5,0xf3,0x2e,0xd5,0x2d,0xf8,0xfb,0xd2,0x12,
        0x7f,0xcf,0x20,0x9d,0x40,0xd1,0x69,0x95,0xab,0xaf,0x94,0x28,0x8b,0xbe,0x2a,0x28,
        0xd2,0x1b,0x12,0x6b,0x05,0x6a,0xdc,0xc4,0x04,0x18,0x4d,0xd3,0x2e,0xd5,0x19,0x75};

    if (argc == 1) {
        printf("\ntest_chacha20 <nbytes> <output>\n\n");
        printf("  <nbytes>  - number of *millions* of output bytes (eg, '8' = '8000000')\n");
        printf("  <output>  - output filename\n\n");
        return;
    }
    
    n = (uint32_t)atol(argv[1]);                        // number of output bytes
    out = (uint8_t *)malloc(N*sizeof(uint8_t));         // output buffer

    chacha20_prng_start(&st);                                 // initialize the rng state
    chacha20_prng_add_entropy((void*)ent, sizeof(ent), &st);  // set the "seed"
    chacha20_prng_ready(&st);                                 // make the generator ready to use

    g = fopen(argv[2],"w");                             
    for(i=0; i<n; i++) {
        chacha20_prng_read((void*)out, N*sizeof(uint8_t), &st);   // assign the number of requested bytes
        fwrite((void*)out, N*sizeof(uint8_t), 1, g);        // write to disk
    }
    fclose(g);
}

