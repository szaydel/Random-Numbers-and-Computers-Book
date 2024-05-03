//
//  file:  rule30_rng.c
//
//  Use Rule 30 to generate random bytes
//
//  compile with:
//    gcc testu01_rule30_rng.c -o testu01_rule30_rng -O3 -Iinclude -Llib -ltestu01 -lprobdist -lmylib -lm
//
//  RTK, 29-Dec-2017
//  Last update:  29-Dec-2017
//
///////////////////////////////////////////////////////////////

#include "TestU01.h"
#include <stdio.h>
#include <stdint.h>

#define N 64
uint64_t state=0x0000000100000000;
uint8_t rule=30;

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

unsigned int rule30_uint32(void) {
    uint32_t v,i,j;
    uint8_t b;
    v = 0;
    for(i=0; i<4; i++) {
        b = 0;
        for(j=0; j<8; j++) {
            b |= ((state>>(N/2))&1)<<(7-j);
            state = apply_rule(rule, state);
        }
        v |= b<<((3-i)*8);
    }
    return (unsigned int)v;
}


int main(int argc, char *argv[]) {
    unif01_Gen *gen;

    if (argc == 1) {
        printf("\ntestu01_rule30_rng <0|1|2>\n\n");
        printf("  <0|1|2>  - 0=SmallCrush, 1=Crush, 2=BigCrush\n\n");
        return 0;
    }

    gen = unif01_CreateExternGenBits("Rule 30", rule30_uint32);

    switch (atoi(argv[1])) {
        case 0:   bbattery_SmallCrush(gen);  break;
        case 1:   bbattery_Crush(gen);       break;
        default:  bbattery_BigCrush(gen);    break;
    }

    unif01_DeleteExternGenBits(gen);
    return 0;
}

