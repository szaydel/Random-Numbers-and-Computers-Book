//
//  file:  lcg_vax.c
//
//  Bit sequences of Vax generator (m=2^32)
//
//  RTK, 24-Jul-2017
//  Last update:  24-Jul-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

#define M 4294967296
#define A 69069
#define C 1

uint32_t seed = 1;

uint32_t randvax(void) {
    seed = (uint32_t)((A*(uint64_t)seed + C) % M);
    return seed;
}

double drandvax(void) {
    return randvax() / (double)M;
}

int main() {
    FILE *f,*f0,*f1,*f2,*f4,*f6,*f8,*f10,*f12,*f14,*f16,*f24,*f31;
    uint8_t b0,b1,b2,b4,b6,b8,b10,b12,b14,b16,b24,b31;
    double d;
    int i,k;
    uint32_t s;

    //  pass to minstd_pi
    f = fopen("double_randvax.dat","w");
    for (i=0; i < 20000000; i++) {
        d = drandvax();
        fwrite((void*)&d, sizeof(double), 1, f);
    }
    fclose(f);

    //  extract bits
    seed = 1;
    f0 = fopen("vax_bit_0.dat", "w");
    f1 = fopen("vax_bit_1.dat", "w");
    f2 = fopen("vax_bit_2.dat", "w");
    f4 = fopen("vax_bit_4.dat", "w");
    f6 = fopen("vax_bit_6.dat", "w");
    f8 = fopen("vax_bit_8.dat", "w");
    f10 = fopen("vax_bit_10.dat","w");
    f12 = fopen("vax_bit_12.dat","w");
    f14 = fopen("vax_bit_14.dat","w");
    f16 = fopen("vax_bit_16.dat", "w");
    f24 = fopen("vax_bit_24.dat", "w");
    f31 = fopen("vax_bit_31.dat", "w");

    for (i=0; i < 4000000; i++) {
        b31 = b24 = b16 = b8 = b6 = b4 = b2 = b1 = b0 = 0;
        for (k=0; k<8; k++) {
            s = randvax();
            b0 <<= 1;
            b1 <<= 1;
            b2 <<= 1;
            b4 <<= 1;
            b6 <<= 1;
            b8 <<= 1;
            b10 <<= 1;
            b12 <<= 1;
            b14 <<= 1;
            b16 <<= 1;
            b24 <<= 1;
            b31 <<= 1;
            b0 |= (s & (1<< 0)) ? 1 : 0;
            b1 |= (s & (1<< 1)) ? 1 : 0;
            b2 |= (s & (1<< 2)) ? 1 : 0;
            b4 |= (s & (1<< 4)) ? 1 : 0;
            b6 |= (s & (1<< 6)) ? 1 : 0;
            b8 |= (s & (1<< 8)) ? 1 : 0;
            b10|= (s & (1<<10)) ? 1 : 0;
            b12|= (s & (1<<12)) ? 1 : 0;
            b14|= (s & (1<<14)) ? 1 : 0;
            b16|= (s & (1<<16)) ? 1 : 0;
            b24|= (s & (1<<24)) ? 1 : 0;
            b31|= (s & (1<<31)) ? 1 : 0;
        }
        fwrite((void*)&b0, 1, 1, f0);
        fwrite((void*)&b1, 1, 1, f1);
        fwrite((void*)&b2, 1, 1, f2);
        fwrite((void*)&b4, 1, 1, f4);
        fwrite((void*)&b6, 1, 1, f6);
        fwrite((void*)&b8, 1, 1, f8);
        fwrite((void*)&b10, 1, 1, f10);
        fwrite((void*)&b12, 1, 1, f12);
        fwrite((void*)&b14, 1, 1, f14);
        fwrite((void*)&b16, 1, 1, f16);
        fwrite((void*)&b24, 1, 1, f24);
        fwrite((void*)&b31, 1, 1, f31);
    }

    fclose(f0);
    fclose(f1);
    fclose(f2);
    fclose(f4);
    fclose(f6);
    fclose(f8);
    fclose(f10);
    fclose(f12);
    fclose(f14);
    fclose(f16);
    fclose(f24);
    fclose(f31);

    return 1;
}

