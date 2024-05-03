//
//  file:  speed_test.c
//
//  Program to time the generation of random doubles.
//
//  -O3 -Irandom123/Random123-1.09/include/ -lrt
//
//  RTK, 17-Dec-2017
//  Last update:  17-Dec-2017
//
///////////////////////////////////////////////////////////////

#include <Random123/threefry.h>
#include <Random123/philox.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>

//  KISS64
uint64_t Q[2097152];                // MWC state
uint64_t carry=0;                   // MWC carry
uint64_t cng=123456789987654321LL;  // LCG state
uint64_t xs=362436069362436069LL;   // xorshift state

//  xorshift
uint64_t xorshift64(void) {
    xs^=(xs<<13);
    xs^=(xs>>17);
    xs^=(xs<<43);
    return xs;
}

//  LCG
uint64_t lcg64(void) {
    cng = 6906969069LL*cng + 13579;
    return cng;
}

//  multiply-with-carry
uint64_t mwc64(void) {
    uint64_t t,x;
    static int j=2097151;
    j=(j+1)&2097151;
    x=Q[j]; 
    t=(x<<28)+carry;
    carry=(x>>36)-(t<x);
    return (Q[j]=t-x);
}

//  KISS
void kiss64_init(uint32_t seed) {
    int i;
    cng = (uint64_t)seed;
    for(i=0; i<2097152; i++) {
        Q[i] = lcg64() + xorshift64();
    }
}

uint64_t kiss64(void) {
    return mwc64() + xorshift64() + lcg64();
}

double dkiss64(void) {
    uint64_t n = (kiss64() >> 12) | 0x3FF0000000000000;
    return *(double *)&n - 1.0;
}

//  xorshift1024*
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

double dxorshift1024star(void) {
    uint64_t n = (xorshift1024star()>>12)|0x3FF0000000000000;
    return *(double *)&n - 1.0;
}


//  MINSTD
#define Ad 48271
#define Qd 44488
#define Rd 3399
#define Md 2147483647
int seed1 = 1;

int minstd1(void) {
    seed1 = Ad * (seed1 % Qd) - Rd * (seed1 / Qd);
    seed1 += (seed1 < 0) ? Md : 0;
    return seed1;
}

double dminstd1(void) {
    return minstd1() / (double)Md;
}

#define A0 16807
#define Q0 127773
#define R0 2836
int seed0 = 1;

int minstd0(void) {
    seed0 = A0 * (seed0 % Q0) - R0 * (seed0 / Q0);
    seed0 += (seed0 < 0) ? Md : 0;
    return seed0;
}

double dminstd0(void) {
    return minstd0() / (double)Md;
}

//  MT
#define N 624
#define M 397
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

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

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

double genrand_real1(void)
{
    return genrand_int32()*(1.0/4294967295.0); 
    /* divided by 2^32-1 */ 
}


////  xorshift128+
uint64_t xs128[] = {2438584823ULL, 983496581ULL};

uint64_t xorshift128plus(void) {
	uint64_t s1 = xs128[0];
	const uint64_t s0 = xs128[1];
	const uint64_t result = s0 + s1;
	xs128[0] = s0;
	s1 ^= s1 << 23; // a
	xs128[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
	return result; 
}

double dxorshift128plus(void) {
    uint64_t n = (xorshift128plus()>>12)|0x3FF0000000000000;
    return *(double *)&n - 1.0;
}

//  xorshift32
unsigned long s32 = 1;

unsigned long xorshift32(void) {
    s32 ^= (s32<<13);
    s32 ^= (s32>>17);
    s32 ^= (s32<<5);
    return s32;
}

double dxorshift32(void) {
    return (double)xorshift32() / 4294967296.0; 
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

double cmwc(void) {
    return (double)CMWC() / 4294967296.0;
}

// middle Weyl
uint64_t wseed = 0xb5ad4ece00000000;

uint32_t middle_weyl(void) {
    static uint64_t x=0, w=0;
    x *= x;
    w += wseed;
    x += w;
    x = (x >> 32) | (x << 32);
    return (uint32_t)x;
}

double dweyl(void) {
    return (double)middle_weyl()/4294967296.0;
}

//  philox
philox4x32_key_t philox32key = {{0,0}};

void philox32_init(uint32_t seed) {
    philox32key.v[0] = seed;
}

uint32_t philox32(void) {
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

double dphilox32(void) {
    return (double)philox32()/4294967296.0;
}


//  threefry
threefry2x64_key_t threefry64key = {{0,0}};

void threefry64_init(uint64_t seed) {
    threefry64key.v[0] = seed;
}

uint64_t threefry64(void) {
    static c=2,i=0;
    static threefry2x64_ctr_t r;
    uint32_t ans;
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
    return ans;
}

double dthreefry64(void) {
    uint64_t n = (threefry64()>>12)|0x3FF0000000000000;
    return *(double *)&n - 1.0;
}


//
//  main
//
int main(int argc, char *argv[]) {
    FILE *f;
    double d;
    int gen, seed, samples, i;
    struct timespec start, end;

    if (argc < 2) {
        printf("\nspeed_test <gen> <samples>\n\n");
        printf("  <gen>     - 0=MINSTD0, 1=MT, 2=xorshift32, 3=CMWC, 4=xorshift1024*, 5=KISS64, 6=xorshift128+,\n");
        printf("              7=middle weyl, 8=philox, 9=threefry\n");
        printf("  <samples> - number of doubles\n\n");
        return 0;
    }

    gen = atoi(argv[1]);
    samples = atoi(argv[2]);

    clock_gettime(CLOCK_MONOTONIC, &start);
    
    // seed
    switch (gen) {
        case 0: seed0 = 12345;        break;             // MINSTD
        case 1: init_genrand(12345);  break;             // Mersenne Twister
        case 2: s32 = 12345;          break;             // xorshift32
        case 3: CMWC_init(12345);     break;             // CMWC
        case 4: xorshift1024star_init(12345); break;     // xorshift1024*
        case 5: kiss64_init(12345);   break;             // KISS64
        case 6: xs128[0] = 12345;     break;             // xorshift128+
        case 7: wseed += 12345; wseed|=1;  break;        // middle weyl
        case 8: philox32_init(12345); break;             // philox
        case 9: threefry64_init(12345); break;           // threefry
        default: break;
    }

    // samples
    for(i=0; i < samples; i++) {
        switch (gen) {
            case 0: d = dminstd0();      break;         // MINSTD
            case 1: d = genrand_real1(); break;         // Mersenne Twister
            case 2: d = dxorshift32();   break;         // xorshift32
            case 3: d = cmwc();          break;         // CMWC
            case 4: d = dxorshift1024star(); break;     // xorshift1024*
            case 5: d = dkiss64();       break;         // KISS64
            case 6: d = dxorshift128plus(); break;         // xorshift128+
            case 7: d = dweyl();         break;         // middle weyl
            case 8: d = dphilox32();     break;         // philox
            case 9: d = dthreefry64();   break;         // threefry
            default: break;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    d = (end.tv_sec + end.tv_nsec*1e-9) - (start.tv_sec + start.tv_nsec*1e-9);
    printf("%0.8f\n", d);

    return 0;
}

