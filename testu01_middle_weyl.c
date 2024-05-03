//
//  file:  testu01_middle_weyl.c
//
//  Apply TestU01 tests to the Middle Weyl generator.
//
//  compile with:
//    gcc testu01_middle_weyl.c -o testu01_middle_weyl -O3 -Iinclude -Llib -ltestu01 -lprobdist -lmylib -lm
//

#include "TestU01.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t seed = 0xb5ad4ece00000000;

unsigned int middle_weyl(void) {
    static uint64_t x=0, w=0;
    x *= x;
    w += seed;
    x += w;
    x = (x >> 32) | (x << 32);
    return (unsigned int)x;
}


int main(int argc, char *argv[]) {
    unif01_Gen *gen;

    if (argc == 1) {
        printf("\ntestu01_middle_weyl <seed> <0|1|2>\n\n");
        printf("  <seed>  - unsigned 32-bit seed\n");
        printf("  <0|1|2> - 0=SmallCrush, 1=Crush, 2=BigCrush\n\n");
        return 0;
    }

    seed |= atol(argv[1]);
    gen = unif01_CreateExternGenBits("Middle Weyl", middle_weyl);

    switch (atoi(argv[2])) {
        case 0:   bbattery_SmallCrush(gen);  break;
        case 1:   bbattery_Crush(gen);       break;
        default:  bbattery_BigCrush(gen);    break;
    }

    unif01_DeleteExternGenBits(gen);
    return 0;
}

