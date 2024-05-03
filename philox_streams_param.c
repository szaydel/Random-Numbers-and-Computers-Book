#include <Random123/philox.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//  xorshift32
unsigned long seed = 1;

unsigned long xorshift32(void) {
    seed ^= (seed<<13);
    seed ^= (seed>>17);
    seed ^= (seed<<5);
    return seed;
}

//  philox
philox4x32_key_t philox32key = {{0,0}};
uint32_t pcount = 0;

void philox32_init(uint32_t seed) {
    philox32key.v[0] = seed;
    pcount = 0;  // reset the counter, too
}

uint32_t philox32(void) {
    static int c=4;
    static philox4x32_ctr_t r;
    uint32_t ans;
    philox4x32_ctr_t ctr={{0,0}};

    if (c==4) {
        ctr.v[0] = pcount;
        r = philox4x32(ctr, philox32key);
        c=1;
        pcount++;
        ans = r.v[0];
    } else {
        ans = r.v[c];
        c++;
    }
    return ans;
}


void main(int argc, char *argv[]) {
    FILE *f;
    char fname[256];
    uint32_t i,j, nsamp, nstreams, u;

    if (argc == 1) {
        printf("\nphilox_streams_param <nstreams> <nsamp> <seed>\n\n");
        printf("  <nstreams> - number of streams to generate\n");
        printf("  <nsamp>    - number of uint32 samples per stream\n");
        printf("  <seed>     - key generator seed value\n\n");
        return;
    }

    nstreams = (uint32_t)atol(argv[1]);
    nsamp = (uint32_t)atol(argv[2]);
    seed = (uint32_t)atol(argv[3]);

    for(i=0; i<nstreams; i++) {
        sprintf(fname,"stream_%03d.dat",i);
        f = fopen(fname,"w");
        //philox32_init(xorshift32());
        philox32_init(i+1);
        for(j=0; j<nsamp; j++) {
            u = philox32();
            fwrite((void*)&u, sizeof(uint32_t), 1, f);
        }
        fclose(f);
    }
}

