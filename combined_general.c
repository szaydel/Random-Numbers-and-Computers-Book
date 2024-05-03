#include <stdio.h>
#include <stdint.h>

// MLCG values for each generator - Table I in L'Ecuyer 1999
#define L 5
int32_t m[] = {2147483647,2147483647,2147483647,2147483647,2147483647};
int32_t a[] = {39373, 742938285, 950706376, 16807, 630360016};
int32_t q[] = {54542, 2, 2, 127773, 3};
int32_t r[] = {1481, 661607077, 246070895, 2836, 256403599};
int32_t s[] = {1802, 6800, 6502, 8080, 4004};

int32_t combined_rng() {
    int32_t Z=0, j, k;

    for(j=0; j<L; j++) {
        k = s[j] / q[j];
        s[j] = a[j] * (s[j] - k*q[j]) - k*r[j];
        if (s[j] < 0) s[j] += m[j];
        if ((j%2)==1)
            Z = (Z - m[0] + 1) + s[j];
        else
            Z = Z - s[j];
        if (Z<1) Z += (m[0]-1);
    }

    return Z;
}

double combined_drng() {
    return (double)combined_rng() / (double)m[0];
}

//  L'Ecuyer's 32-bit generator
int32_t s1 = 12345;
int32_t s2 = 53211;

int32_t combined_rng2() {
    int32_t Z,k;

    k = s1 / 53668;
    s1 = 40014 * (s1 - k * 53668) - k * 12211;
    if (s1 < 0) s1 += 2147483563;
    k = s2 / 52774;
    s2 = 40692 * (s2 - k * 52774) - k * 3791;
    if (s2 < 0) s2 += 2147483399;
    Z = s1-s2;
    if (Z < 1) Z += 2147483562;
    return Z;
}

double combined_drng2() {
    return (double)combined_rng2()*4.656612873077393e-10;
}


void main() {
    int i;
    FILE *g;
    uint32_t b;

    g = fopen("random_combined.dat", "w");
    for(i=0; i<10000000; i++) {
        b = (uint32_t)combined_rng();
        fwrite((void *)&b, sizeof(uint8_t), 3, g);
    }
    fclose(g);

    g = fopen("random_two_combined.dat", "w");
    for(i=0; i<10000000; i++) {
        b = (uint32_t)combined_rng2();
        fwrite((void *)&b, sizeof(uint8_t), 3, g);
    }
    fclose(g);
}

