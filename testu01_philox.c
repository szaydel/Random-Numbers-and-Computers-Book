//
//  file:  testu01_philox.c
//
//  Apply TestU01 tests to the Philox generator.
//
//  compile with:
//    gcc testu01_philox.c -o testu01_philox -O3 -ITestU01-1.2.3/include -Irandom123/Random123-1.09/include 
//                         -Llib -ltestu01 -lprobdist -lmylib -lm
//

#include "TestU01.h"
#include <Random123/philox.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

philox4x32_key_t philox32key = {{0,0}};

void philox32_init(uint32_t seed) {
    philox32key.v[0] = seed;
}

unsigned int philox32(void) {
    static c=4,i=0;
    static philox4x32_ctr_t r;
    uint32_t ans;
    philox4x32_ctr_t ctr={{0,0}};

    if (c==4) {
        ctr.v[0] = i;
        r = philox4x32(ctr, philox32key);
        c=1;
        i++;
        ans = r.v[0];
    } else {
        ans = r.v[c];
        c++;
    }
    return ans;
}


int main(int argc, char *argv[]) {
    unif01_Gen *gen;

    if (argc == 1) {
        printf("\ntestu01_philox <seed> <0|1|2>\n\n");
        printf("  <seed>  - unsigned 32-bit seed\n");
        printf("  <0|1|2> - 0=SmallCrush, 1=Crush, 2=BigCrush\n\n");
        return 0;
    }

    philox32_init(atol(argv[1]));
    gen = unif01_CreateExternGenBits("Philox", philox32);

    switch (atoi(argv[2])) {
        case 0:   bbattery_SmallCrush(gen);  break;
        case 1:   bbattery_Crush(gen);       break;
        default:  bbattery_BigCrush(gen);    break;
    }

    unif01_DeleteExternGenBits(gen);
    return 0;
}

