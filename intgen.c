//
//  file:  intgen.c
//
//  Make data files for randomness tests
//
//  compile with:
//    -lm -D _LARGEFILE64_SOURCE -D _FILE_OFFSET_BITS=64
//  to enable large file support!
//  
//  RTK, 24-Oct-2017
//  Last update:  03-Nov-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

//  MINSTD orig
#define M 2147483647
#define A0 16807
#define Q0 127773
#define R0 2836

int seed0 = 1;
int minstd0(void) {
    seed0 = A0 * (seed0 % Q0) - R0 * (seed0 / Q0);
    seed0 += (seed0 < 0) ? M : 0;
    return seed0;
}

//  MINSTD revised
#define A1 48271
#define Q1 44488
#define R1 3399

int seed1 = 1;
int minstd1(void) {
    seed1 = A1 * (seed1 % Q1) - R1 * (seed1 / Q1);
    seed1 += (seed1 < 0) ? M : 0;
    return seed1;
}

//  RANDU
uint32_t srandu = 1;
uint32_t randu(void) {
    srandu = 65539*srandu % 2147483648;
}


//  xorshift128+ (64)
uint64_t s[2];

void xorshift128plus_init(uint64_t seed) {
    s[0] = seed;
    s[1] = seed^0xed34120f;
}

uint64_t xorshift128plus(void) {
	uint64_t s1 = s[0];
	const uint64_t s0 = s[1];
	const uint64_t result = s0 + s1;
	s[0] = s0;
	s1 ^= s1 << 23; // a
	s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
	return result; 
}

//  xorshift1024* (64)
uint64_t s1024[16]; 
int p1024;

void xorshift1024star_init(uint64_t seed) {
    int i;
    s1024[0] = seed;
    for(i=1; i<16; i++)
        s1024[i] = s1024[i-1]*seed+seed;
}

uint64_t xorshift1024star(void) {
	const uint64_t s0 = s1024[p1024];
	uint64_t s1 = s1024[p1024 = (p1024 + 1) & 15];
	s1 ^= s1 << 31; // a
	s1024[p1024] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30); // b,c
	return s1024[p1024] * UINT64_C(1181783497276652981);
}

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

unsigned long s128[] = {123456789,362436069,521288629,88675123};

#define A128 11
#define B128  8
#define C128 19

void xorshift128_init(uint32_t seed) {
    s128[0] = (unsigned long)seed;
}

uint32_t xorshift128(void) {
    unsigned long t;
    t = (s128[0]^(s128[0]<<A128));
    s128[0]=s128[1]; s128[1]=s128[2]; s128[2]=s128[3];
    s128[3] = (s128[3] ^ (s128[3]>>C128)) ^ (t^(t>>B128));
    return (uint32_t)s128[3];
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

uint32_t z1 = 0xff32422;
uint32_t z2 = 0xee03202;
uint32_t z3 = 0xcc23423;
uint32_t z4 = 0x1235;  // can update this as the seed

uint32_t TausStep(uint32_t *z, uint32_t S1, uint32_t S2, uint32_t S3, uint32_t MM)
{
    uint32_t b = ((*z << S1) ^ *z) << S2; 
    *z = (((*z & MM) << S3) ^ b); 
    return *z; 
}

uint32_t LCGStep(uint32_t *z, uint32_t A, uint32_t C)
{
    *z = (A*(*z)+C);
    return *z; 
}

uint32_t HybridTaus()
{
    return TausStep(&z1, 13, 19, 12, 4294967294UL) ^
           TausStep(&z2, 2, 25, 4, 4294967288UL) ^
           TausStep(&z3, 3, 11, 17, 4294967280UL) ^
           LCGStep(&z4, 1664525, 1013904223UL);
}

int main(int argc, char *argv[]) {
    FILE *f;
    uint32_t u32;
    uint64_t u64,i,samples;
    int gen, seed;

    if (argc < 2) {
        printf("\nintgen <gen> <seed> <samples> <output>\n\n");
        printf("  <gen>     - 0=MINSTD(orig), 1=MINSTD(new), 2=xorshift128+, 3=xorshift1024*, 4=RANDU,");
        printf(" 5=xorshift32, 6=CMWC, 7=xorshift128\n              8=KISS32, 9=Middle Weyl, 10=MT, 11=Combined LCG, 12=hybrid Taus\n");
        printf("  <seed>    - 32-bit integer seed\n");
        printf("  <samples> - number of output values\n");
        printf("  <output>  - name of the output file\n\n");
        return 0;
    }

    gen = atoi(argv[1]);
    seed = atoi(argv[2]);
    samples = (uint64_t)atoll(argv[3]);
    
    f = fopen(argv[4],"w");

    // seed
    switch (gen) {
        case 0: seed0 = seed;                break;
        case 1: seed1 = seed;                break;
        case 2: xorshift128plus_init(seed);  break;
        case 3: xorshift1024star_init(seed); break;
        case 4: srandu = seed;               break;
        case 5: s32 = seed;                  break;
        case 6: CMWC_init(seed);             break;
        case 7: xorshift128_init(seed);      break;
        case 8: kiss32_init(seed);           break;
        case 9: mwseed |= seed;              break;
        case 10: init_genrand(seed);         break;
        case 11: s2 = (int32_t)seed;         break;
        case 12: z4 = seed;                  break;
        default: break;
    }

    // samples
    for(i=0; i < samples; i++) {
        switch (gen) {
            case 0: u32 = minstd0();          fwrite((void*)&u32, sizeof(uint32_t), 1, f); break;
            case 1: u32 = minstd1();          fwrite((void*)&u32, sizeof(uint32_t), 1, f); break;
            case 2: u64 = xorshift128plus();  fwrite((void*)&u64, sizeof(uint64_t), 1, f); break;
            case 3: u64 = xorshift1024star(); fwrite((void*)&u64, sizeof(uint64_t), 1, f); break;
            case 4: u32 = randu();            fwrite((void*)&u32, sizeof(uint32_t), 1, f); break;
            case 5: u32 = xorshift32();       fwrite((void*)&u32, sizeof(uint32_t), 1, f); break;
            case 6: u32 = CMWC();             fwrite((void*)&u32, sizeof(uint32_t), 1, f); break;
            case 7: u32 = xorshift128();      fwrite((void*)&u32, sizeof(uint32_t), 1, f); break;
            case 8: u32 = kiss32();           fwrite((void*)&u32, sizeof(uint32_t), 1, f); break;
            case 9: u32 = middle_weyl();      fwrite((void*)&u32, sizeof(uint32_t), 1, f); break;
            case 10: u32 = genrand_int32();   fwrite((void*)&u32, sizeof(uint32_t), 1, f); break;
            case 11: u32 = uint32_combined_rng2();   fwrite((void*)&u32, sizeof(uint32_t), 1, f); break;
            case 12: u32 = HybridTaus();      fwrite((void*)&u32, sizeof(uint32_t), 1, f); break;
            default: break;
        }
    }

    fclose(f);
    return 0;
}
