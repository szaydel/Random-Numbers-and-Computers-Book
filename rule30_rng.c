//
//  file:  rule30_rng.c
//
//  Use Rule 30 to generate random bytes
//
//  RTK, 29-Dec-2017
//  Last update:  29-Dec-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

#define N 64

uint64_t apply_rule(uint8_t rule, uint64_t state) {
    uint64_t nstate=0, j;

    for(j=0; j<N; j++) {
        switch (j) {
            case 0:
                if (rule & (1<<(((state&3)<<1)|((state>>(N-1))&1))))
                    nstate |= 1;
                break;
            case (N-1):
                if (rule & (1<<(((state>>(N-2))&3)|((state&1)<<2))))
                    nstate |= (1ULL<<(N-1));
                break;
            default:
                if (rule & (1<<((state>>(j-1))&7)))
                    nstate |= (1ULL<<j);
                break;
        }
    }
    return nstate;
}


void main(int argc, char *argv[]) {
    FILE *f;
    uint8_t b, rule=30;
    uint32_t nsamp,i,j;
    uint64_t state=0x0000000100000000;

    if (argc == 1) {
        printf("\nrule30_rng <nsamp> <output>\n\n");
        printf("  <nsamp>  - number of uint8 samples\n");
        printf("  <output> - output binary file name\n\n");
        return;
    }

    nsamp = (uint64_t)atol(argv[1]);
    f = fopen(argv[2],"w");

    for(i=0; i<nsamp; i++) {
        b = 0;
        for(j=0; j<8; j++) {
            b |= ((state>>(N/2))&1)<<(7-j);
            state = apply_rule(rule, state);
        }
        fwrite((void*)&b, sizeof(uint8_t), 1, f);
    }

    fclose(f);
}

