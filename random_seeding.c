//
//  file:  random_seeding.c
//
//  Random seeding output files
//
//  compile with:
//    -lm -D _LARGEFILE64_SOURCE -D _FILE_OFFSET_BITS=64
//  to enable large file support!
//  
//  RTK, 14-Nov-2017
//  Last update:  14-Nov-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

//  xorshift32
unsigned long s32 = 1;

unsigned long xorshift32(void) {
    s32 ^= (s32<<13);
    s32 ^= (s32>>17);
    s32 ^= (s32<<5);
    return s32;
}

//  CMWC
#define Rc 4096
#define Ac 18782ULL

uint32_t Qc[Rc];
uint32_t c=362436; // c < 809430660

void CMWC_init(uint32_t seed) {
    int i;
    s32 = seed;
    for(i=0; i<Rc; i++)
        Qc[i] = xorshift32();
}

uint32_t CMWC(void) {
    static uint32_t i=Rc-1;
    uint64_t t;
    uint32_t x, m=0xFFFFFFFE;

    i = (i+1) & (Rc-1);
    t = Ac*Qc[i] + c;
    c = t >> 32;
    x = t+c;
    if (x < c) {
        x++; c++;
    }
    Qc[i] = m-x;
    return Qc[i];
}

// KISS32
uint32_t Q[4194304];    // MWC state
uint32_t carry=0;       // MWC carry
uint32_t cng=123456789; // LCG state
uint32_t xs=362436069;  // xorshift state

//  xorshift
uint32_t xshift32(void) {
    xs^=(xs<<13);
    xs^=(xs>>17);
    xs^=(xs<<5);
    return xs;
}

//  LCG
uint32_t lcg32(void) {
    cng = 69069*cng + 13579;
    return cng;
}

//  multiply-with-carry
uint32_t mwc32(void) {
    uint32_t t,x;
    static int j=4194303;

    j=(j+1)&4194303;
    x=Q[j]; 
    t=(x<<28)+carry;
    carry=(x>>4)-(t<x);

    return (Q[j]=t-x);
}

//  KISS
void kiss32_init(uint32_t seed) {
    int i;
    xs = seed;
    for(i=0; i<4194304; i++) {
        Q[i] = lcg32() + xshift32();
    }
}

uint32_t kiss32(void) {
    return mwc32() + xshift32() + lcg32();
}

uint64_t mwseed = 0xb5ad4ece00000000;

uint32_t middle_weyl(void) {
    static uint64_t x=0, w=0;
    x *= x;
    w += mwseed;
    x += w;
    x = (x >> 32) | (x << 32);
    return (uint32_t)x;
}

//  MT
#define N 624
#define TM 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

unsigned long genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-TM;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+TM] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(TM-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[TM-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

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

uint32_t uint32_combined_rng2() {
    return (uint32_t)(4294967296.0*((double)combined_rng2()/2147483647.0));
}

int main(int argc, char *argv[]) {
    int i,j,sgen,dgen,nstream,nsamp,sseed;
    FILE *f;
    char fname[256];
    uint32_t *seeds, u32;

    if (argc < 2) {
        printf("\nrandom_seeding <sgen> <dgen> <nstream> <nsamp> <sseed>\n\n");
        printf("  <sgen>    - source generator number\n");
        printf("  <dgen>    - destination generator number\n");
        printf("  <nstream> - number independent streams\n");
        printf("  <nsamp>   - number of samples per stream\n");
        printf("  <sseed>   - source generator uint32 seed\n\n");
        printf("Generators: 0=CMWC, 1=MT, 2=KISS, 3=msws, 4=combined\n\n");
        return 0;
    }

    sgen = atoi(argv[1]);
    dgen = atoi(argv[2]);
    nstream = atoi(argv[3]);
    nsamp = atoi(argv[4]);
    sseed = atoi(argv[5]);

    //  Seed source generator
    switch (sgen) {
        case 0:  CMWC_init(sseed);     break;
        case 1:  init_genrand(sseed);  break;
        case 2:  kiss32_init(sseed);   break;
        case 3:  mwseed |= sseed;      break;
        default: s2 = (int32_t)sseed;  break; 
    }

    //  Generate destination generator seeds
    seeds = (uint32_t *)malloc(nstream*sizeof(uint32_t));
    for(i=0; i<nstream; i++) {
        switch (sgen) {
            case 0:  seeds[i]=CMWC();           break;
            case 1:  seeds[i]=genrand_int32();  break;
            case 2:  seeds[i]=kiss32();         break;
            case 3:  seeds[i]=middle_weyl();    break;
            default: seeds[i]=uint32_combined_rng2();  break;
        }
    }

    //  Generate the streams using the destination generator
    for(i=0; i < nstream; i++) {
        sprintf(fname,"stream_%03d.dat",i);
        f = fopen(fname,"w");

        //  Seed the destination generator
        switch (dgen) {
            case 0:  CMWC_init(seeds[i]);     break;
            case 1:  init_genrand(seeds[i]);  break;
            case 2:  kiss32_init(seeds[i]);   break;
            case 3:  mwseed |= seeds[i];      break;
            default: s2 = (int32_t)seeds[i];  break; 
        }

        //  Generate the stream samples
        for(j=0; j<nsamp; j++) {
            switch (dgen) {
                case 0:  u32 = CMWC();           break;
                case 1:  u32 = genrand_int32();  break;
                case 2:  u32 = kiss32();         break;
                case 3:  u32 = middle_weyl();    break;
                default: u32 =uint32_combined_rng2();  break;
            }
            fwrite((void*)&u32, sizeof(uint32_t), 1, f);
        }

        fclose(f);
    }

    return 0;
}
