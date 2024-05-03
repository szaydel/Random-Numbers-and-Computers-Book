//
//  file:  testu01_logistic_three.c
//
//  Apply TestU01 tests to the Logistic Three generator.
//
//  compile with:
//    gcc testu01_logistic_three.c -o testu01_logistic_three -O3 -Iinclude -Llib -ltestu01 -lprobdist -lmylib -lm
//

#include "TestU01.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

double r=3.9999, x=0.2, y=0.3;

unsigned int logistic_three(void) {
    unsigned int u0,u1;
    double xt = r*x*(1.0-x);
    double yt = r*y*(1.0-y);
    x = yt;
    y = xt;
    u0 = (unsigned int)(0xFFFFFFFF*xt);
    u1 = (unsigned int)(0xFFFFFFFF*yt);
    u1 = (u1>>24) | (u1<<24) | ((u1>>16)&0xff)<<8 | ((u1>>8)&0xff)<<16;
    u0 ^= u1;
    return u0;
}


int main(int argc, char *argv[]) {
    unif01_Gen *gen;
    int i;

    if (argc == 1) {
        printf("\ntestu01_logistic_three <r> <x> <y> <0|1|2>\n\n");
        printf("  <r>      - logistic map r value, [3.9,4.0)\n");
        printf("  <x>,<y>  - 'seed' values\n");
        printf("  <0|1|2>  - 0=SmallCrush, 1=Crush, 2=BigCrush\n\n");
        return 0;
    }

    sscanf(argv[1],"%lf",&r);
    sscanf(argv[2],"%lf",&x);
    sscanf(argv[3],"%lf",&y);

    //  Burn-in
    for(i=0; i<1000; i++) {
        x = r*x*(1.0-x);
        y = r*y*(1.0-y);
    }

    gen = unif01_CreateExternGenBits("Logistic Three", logistic_three);

    switch (atoi(argv[4])) {
        case 0:   bbattery_SmallCrush(gen);  break;
        case 1:   bbattery_Crush(gen);       break;
        default:  bbattery_BigCrush(gen);    break;
    }

    unif01_DeleteExternGenBits(gen);
    return 0;
}

