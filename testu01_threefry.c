//
//  file:  testu01_threefry.c
//
//  Apply TestU01 tests to the Threefry generator.
//
//  compile with:
//    gcc testu01_threefry.c -o testu01_threefry -O3 -ITestU01-1.2.3/include -Irandom123/Random123-1.09/include 
//                         -Llib -ltestu01 -lprobdist -lmylib -lm
//

#include "TestU01.h"
#include <Random123/threefry.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

threefry2x64_key_t threefry64key = {{0,0}};

void threefry64_init(uint64_t seed) {
    threefry64key.v[0] = seed;
}

unsigned int threefry64(void) {
    static c=2,i=0;
    static threefry2x64_ctr_t r;
    uint64_t ans;
    threefry2x64_ctr_t ctr={{0,0}};

    if (c==2) {
        ctr.v[0] = i;
        r = threefry2x64(ctr, threefry64key);
        c=1;
        i++;
        ans = r.v[0];
    } else {
        ans = r.v[c];
        c++;
    }
    return (unsigned int)(ans>>32);
}


int main(int argc, char *argv[]) {
    unif01_Gen *gen;

    if (argc == 1) {
        printf("\ntestu01_threefry <seed> <0|1|2>\n\n");
        printf("  <seed>  - unsigned 32-bit seed\n");
        printf("  <0|1|2> - 0=SmallCrush, 1=Crush, 2=BigCrush\n\n");
        return 0;
    }

    threefry64_init(atol(argv[1]));
    gen = unif01_CreateExternGenBits("Threefry", threefry64);

    switch (atoi(argv[2])) {
        case 0:   bbattery_SmallCrush(gen);  break;
        case 1:   bbattery_Crush(gen);       break;
        default:  bbattery_BigCrush(gen);    break;
    }

    unif01_DeleteExternGenBits(gen);
    return 0;
}

