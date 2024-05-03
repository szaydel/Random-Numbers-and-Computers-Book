//
//  file: minstd.c
//
//  MINSTD with driver.
//
//  RTK, 24-Jul-2017
//  Last update:  24-Jul-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>

#define M 2147483647
#define A0 16807
#define Q0 127773
#define R0 2836
#define A1 48271
#define Q1 44488
#define R1 3399

int seed0 = 1;
int seed1 = 1;

int minstd0(void) {
    seed0 = A0 * (seed0 % Q0) - R0 * (seed0 / Q0);
    seed0 += (seed0 < 0) ? M : 0;
    return seed0;
}

double dminstd0(void) {
    return minstd0() / (double)M;
}

int minstd1(void) {
    seed1 = A1 * (seed1 % Q1) - R1 * (seed1 / Q1);
    seed1 += (seed1 < 0) ? M : 0;
    return seed1;
}

double dminstd1(void) {
    return minstd1() / (double)M;
}


int main() {
    FILE *f, *f0, *f1, *f4, *f20, *f23;
    unsigned char b0,b1,b4,b20,b23;
    double d;
    int i,k,s;

    //  20,000,000 doubles - original MINSTD
    seed0 = 1;
    f = fopen("doubles_minstd_orig.dat","w");
    for (i=0; i < 20000000; i++) {
        d = dminstd0();
        fwrite((void*)&d, sizeof(double), 1, f);
    }
    fclose(f);

    //  20,000,000 doubles - updated MINSTD
    seed1 = 1;
    f = fopen("doubles_minstd_updated.dat","w");
    for (i=0; i < 20000000; i++) {
        d = dminstd1();
        fwrite((void*)&d, sizeof(double), 1, f);
    }
    fclose(f);

    //  4,000,000*8 bytes, bits 0, 1, 4, 20, 23 - original MINSTD
    seed0 = 1;
    f0 = fopen("minstd_orig_bytes_bit_0.dat","w");
    f1 = fopen("minstd_orig_bytes_bit_1.dat","w");
    f4 = fopen("minstd_orig_bytes_bit_4.dat","w");
    f20= fopen("minstd_orig_bytes_bit_20.dat","w");
    f23= fopen("minstd_orig_bytes_bit_23.dat","w");

    for (i=0; i < 4000000; i++) {
        b23 = b20 = b4 = b1 = b0 = 0;
        for (k=0; k<8; k++) {
            s = minstd0();
            b0 <<= 1;
            b1 <<= 1;
            b4 <<= 1;
            b20 <<= 1;
            b23 <<= 1;
            b0 |= (s & (1<< 0)) ? 1 : 0;
            b1 |= (s & (1<< 1)) ? 1 : 0;
            b4 |= (s & (1<< 4)) ? 1 : 0;
            b20|= (s & (1<<20)) ? 1 : 0;
            b23|= (s & (1<<23)) ? 1 : 0;
        }
        fwrite((void*)&b0, 1, 1, f0);
        fwrite((void*)&b1, 1, 1, f1);
        fwrite((void*)&b4, 1, 1, f4);
        fwrite((void*)&b20, 1, 1, f20);
        fwrite((void*)&b23, 1, 1, f23);
    }

    fclose(f0);
    fclose(f1);
    fclose(f4);
    fclose(f20);
    fclose(f23);

    //  4,000,000*8 bytes, bits 0, 1, 4, 20, 23 - updated MINSTD
    seed1 = 1;
    f0 = fopen("minstd_updated_bytes_bit_0.dat","w");
    f1 = fopen("minstd_updated_bytes_bit_1.dat","w");
    f4 = fopen("minstd_updated_bytes_bit_4.dat","w");
    f20= fopen("minstd_updated_bytes_bit_20.dat","w");
    f23= fopen("minstd_updated_bytes_bit_23.dat","w");

    for (i=0; i < 4000000; i++) {
        b23 = b20 = b4 = b1 = b0 = 0;
        for (k=0; k<8; k++) {
            s = minstd1();
            b0 <<= 1;
            b1 <<= 1;
            b4 <<= 1;
            b20 <<= 1;
            b23 <<= 1;
            b0 |= (s & (1<< 0)) ? 1 : 0;
            b1 |= (s & (1<< 1)) ? 1 : 0;
            b4 |= (s & (1<< 4)) ? 1 : 0;
            b20|= (s & (1<<20)) ? 1 : 0;
            b23|= (s & (1<<23)) ? 1 : 0;
        }
        fwrite((void*)&b0, 1, 1, f0);
        fwrite((void*)&b1, 1, 1, f1);
        fwrite((void*)&b4, 1, 1, f4);
        fwrite((void*)&b20, 1, 1, f20);
        fwrite((void*)&b23, 1, 1, f23);
    }

    fclose(f0);
    fclose(f1);
    fclose(f4);
    fclose(f20);
    fclose(f23);

    return 1;
}

