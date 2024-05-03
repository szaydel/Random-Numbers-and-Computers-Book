#include <Random123/philox.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//  philox
philox4x32_key_t philox32key = {{0,0}};

void philox32_init(uint32_t seed) {
    philox32key.v[0] = seed;
}

uint32_t philox32(uint32_t j) {
    philox4x32_ctr_t r;
    philox4x32_ctr_t ctr={{0,0}};

    ctr.v[0] = j;
    r = philox4x32(ctr, philox32key);
    return r.v[0];
}


void main(int argc, char *argv[]) {
    FILE *f;
    char fname[256];
    uint32_t i,j, nsamp, nstreams, u, key;
    uint32_t begin, end;

    if (argc == 1) {
        printf("\nphilox_streams_param <nstreams> <nsamp> <seed>\n\n");
        printf("  <nstreams> - number of streams to generate\n");
        printf("  <nsamp>    - number of uint32 samples per stream\n");
        printf("  <key>      - key value\n\n");
        return;
    }

    nstreams = (uint32_t)atol(argv[1]);
    nsamp = (uint32_t)atol(argv[2]);
    key = (uint32_t)atol(argv[3]);
    
    philox32_init(key);

    for(i=0; i<nstreams; i++) {
        sprintf(fname,"stream_%03d.dat",i);
        f = fopen(fname,"w");
        begin = i*nsamp;
        end = begin + nsamp;
        for(j=begin; j<end; j++) {
            u = philox32(j);
            fwrite((void*)&u, sizeof(uint32_t), 1, f);
        }
        fclose(f);
    }
}

