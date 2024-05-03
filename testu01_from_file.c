//
//  file:  testu01_from_file.c
//
//  Apply TestU01 tests to a file of bytes
//
//  compile with:
//    gcc testu01_from_file.c -o testu01_from_file -O3 -Iinclude -Llib -ltestu01 -lprobdist -lmylib -lm
//

#include "TestU01.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

FILE *g;
int rewinds = 0;

unsigned int fromfile(void) {
    unsigned int u32;    
    
    if (fread((void*)&u32, sizeof(unsigned int), 1, g) == 0) {
        rewind(g);
        rewinds++;
        fread((void*)&u32, sizeof(unsigned int), 1, g);
    }
    return u32;
}


int main(int argc, char *argv[]) {
    unif01_Gen *gen;

    if (argc == 1) {
        printf("\ntestu01_from_file <0|1|2> <fname>\n\n");
        printf("  <0|1|2> - 0=SmallCrush, 1=Crush, 2=BigCrush\n");
        printf("  <file>  - path to file of bytes\n\n");
        return 0;
    }

    gen = unif01_CreateExternGenBits("From file", fromfile);
    g = fopen(argv[2],"r");

    switch (atoi(argv[1])) {
        case 0:   bbattery_SmallCrush(gen);  break;
        case 1:   bbattery_Crush(gen);       break;
        default:  bbattery_BigCrush(gen);    break;
    }

    printf("\n\ninput file rewound %d times\n\n", rewinds);

    unif01_DeleteExternGenBits(gen);
    fclose(g);

    return 0;
}

