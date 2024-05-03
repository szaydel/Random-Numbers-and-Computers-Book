//
//  file:  xor_streams.c
//
//  Fog paper method 7
//
//  RTK, 07-Dec-2017
//  Last update:  07-Dec-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

//  first generators

//  xorshift32
unsigned long s320 = 1;

unsigned long xorshift320(void) {
    s320 ^= (s320<<13);
    s320 ^= (s320>>17);
    s320 ^= (s320<<5);
    return s320;
}

//  CMWC
#define Rc0 4096
#define Ac0 18782ULL

uint32_t Qc0[Rc0];
uint32_t c0=362436; // c < 809430660

void CMWC_init0(uint32_t seed) {
    int i;
    s320 = seed;
    for(i=0; i<Rc0; i++)
        Qc0[i] = xorshift320();
}

uint32_t CMWC0(void) {
    static uint32_t i=Rc0-1;
    uint64_t t;
    uint32_t x, m=0xFFFFFFFE;

    i = (i+1) & (Rc0-1);
    t = Ac0*Qc0[i] + c0;
    c0 = t >> 32;
    x = t+c0;
    if (x < c0) {
        x++; c0++;
    }
    Qc0[i] = m-x;
    return Qc0[i];
}

// KISS32
uint32_t Q0[4194304];    // MWC state
uint32_t carry0=0;       // MWC carry
uint32_t cng0=123456789; // LCG state
uint32_t xs0=362436069;  // xorshift state

//  xorshift
uint32_t xshift320(void) {
    xs0^=(xs0<<13);
    xs0^=(xs0>>17);
    xs0^=(xs0<<5);
    return xs0;
}

//  LCG
uint32_t lcg320(void) {
    cng0 = 69069*cng0 + 13579;
    return cng0;
}

//  multiply-with-carry
uint32_t mwc320(void) {
    uint32_t t,x;
    static int j=4194303;

    j=(j+1)&4194303;
    x=Q0[j]; 
    t=(x<<28)+carry0;
    carry0=(x>>4)-(t<x);

    return (Q0[j]=t-x);
}

//  KISS
void kiss32_init0(uint32_t seed) {
    int i;
    xs0 = seed;
    for(i=0; i<4194304; i++) {
        Q0[i] = lcg320() + xshift320();
    }
}

uint32_t kiss320(void) {
    return mwc320() + xshift320() + lcg320();
}

//  MT
#define N 624
#define TM 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt0[N]; /* the array for the state vector  */
static int mti0=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void init_genrand0(unsigned long s)
{
    mt0[0]= s & 0xffffffffUL;
    for (mti0=1; mti0<N; mti0++) {
        mt0[mti0] = 
	    (1812433253UL * (mt0[mti0-1] ^ (mt0[mti0-1] >> 30)) + mti0); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt0[mti0] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

unsigned long genrand_int320(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti0 >= N) { /* generate N words at one time */
        int kk;

        if (mti0 == N+1)   /* if init_genrand() has not been called, */
            init_genrand0(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-TM;kk++) {
            y = (mt0[kk]&UPPER_MASK)|(mt0[kk+1]&LOWER_MASK);
            mt0[kk] = mt0[kk+TM] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt0[kk]&UPPER_MASK)|(mt0[kk+1]&LOWER_MASK);
            mt0[kk] = mt0[kk+(TM-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt0[N-1]&UPPER_MASK)|(mt0[0]&LOWER_MASK);
        mt0[N-1] = mt0[TM-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti0 = 0;
    }
  
    y = mt0[mti0++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

uint64_t mwseed0 = 0xb5ad4ece00000000;

uint32_t middle_weyl0(void) {
    static uint64_t x=0, w=0;
    x *= x;
    w += mwseed0;
    x += w;
    x = (x >> 32) | (x << 32);
    return (uint32_t)x;
}

//
//  second generators
//
//  xorshift32
unsigned long s321 = 1;

unsigned long xorshift321(void) {
    s321 ^= (s321<<13);
    s321 ^= (s321>>17);
    s321 ^= (s321<<5);
    return s321;
}

//  CMWC
#define Rc1 4096
#define Ac1 18782ULL

uint32_t Qc1[Rc1];
uint32_t c1=362436; // c < 809430660

void CMWC_init1(uint32_t seed) {
    int i;
    s321 = seed;
    for(i=0; i<Rc1; i++)
        Qc1[i] = xorshift321();
}

uint32_t CMWC1(void) {
    static uint32_t i=Rc1-1;
    uint64_t t;
    uint32_t x, m=0xFFFFFFFE;

    i = (i+1) & (Rc1-1);
    t = Ac1*Qc1[i] + c1;
    c1 = t >> 32;
    x = t+c1;
    if (x < c1) {
        x++; c1++;
    }
    Qc1[i] = m-x;
    return Qc1[i];
}

// KISS32
uint32_t Q1[4194304];    // MWC state
uint32_t carry1=0;       // MWC carry
uint32_t cng1=123456789; // LCG state
uint32_t xs1=362436069;  // xorshift state

//  xorshift
uint32_t xshift321(void) {
    xs1^=(xs1<<13);
    xs1^=(xs1>>17);
    xs1^=(xs1<<5);
    return xs1;
}

//  LCG
uint32_t lcg321(void) {
    cng1 = 69069*cng1 + 13579;
    return cng1;
}

//  multiply-with-carry
uint32_t mwc321(void) {
    uint32_t t,x;
    static int j=4194303;

    j=(j+1)&4194303;
    x=Q1[j]; 
    t=(x<<28)+carry1;
    carry1=(x>>4)-(t<x);

    return (Q1[j]=t-x);
}

//  KISS
void kiss32_init1(uint32_t seed) {
    int i;
    xs1 = seed;
    for(i=0; i<4194304; i++) {
        Q1[i] = lcg321() + xshift321();
    }
}

uint32_t kiss321(void) {
    return mwc321() + xshift321() + lcg321();
}

//  MT
//#define N 624
//#define TM 397
//#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
//#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
//#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt1[N]; /* the array for the state vector  */
static int mti1=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void init_genrand1(unsigned long s)
{
    mt1[0]= s & 0xffffffffUL;
    for (mti1=1; mti1<N; mti1++) {
        mt1[mti1] = 
	    (1812433253UL * (mt1[mti1-1] ^ (mt1[mti1-1] >> 30)) + mti1); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt1[mti1] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

unsigned long genrand_int321(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti1 >= N) { /* generate N words at one time */
        int kk;

        if (mti1 == N+1)   /* if init_genrand() has not been called, */
            init_genrand1(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-TM;kk++) {
            y = (mt1[kk]&UPPER_MASK)|(mt1[kk+1]&LOWER_MASK);
            mt1[kk] = mt1[kk+TM] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt1[kk]&UPPER_MASK)|(mt1[kk+1]&LOWER_MASK);
            mt1[kk] = mt1[kk+(TM-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt1[N-1]&UPPER_MASK)|(mt1[0]&LOWER_MASK);
        mt1[N-1] = mt1[TM-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti1 = 0;
    }
  
    y = mt1[mti1++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

uint64_t mwseed1 = 0xb5ad4ece00000000;

uint32_t middle_weyl1(void) {
    static uint64_t x=0, w=0;
    x *= x;
    w += mwseed1;
    x += w;
    x = (x >> 32) | (x << 32);
    return (uint32_t)x;
}

//
//  seed generators
//
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

//  RANDU
uint32_t srandu = 1;
uint32_t randu(void) {
    srandu = 65539*srandu % 2147483648;
}



//
//  main
//
int main(int argc, char *argv[]) {
    int i,j,zgen,sgen,dgen,nstream,nsamp;
    FILE *f;
    char fname[256];
    uint32_t sseed,u;

    if (argc < 2) {
        printf("\nxor_streams <s1gen> <s2gen> <nstream> <nsamp> 0|1 <sseed>\n\n");
        printf("  <s1gen>   - source generator number\n");
        printf("  <s2gen>   - destination generator number\n");
        printf("  <nstream> - number independent streams\n");
        printf("  <nsamp>   - number of samples per stream\n");
        printf("  0|1       - 0=MINSTD(orig), 1=RANDU\n");
        printf("  <sseed>   - MINSTD|RANDU seed\n\n");
        printf("Generators: 0=CMWC, 1=MT, 2=KISS32, 3=Middle Weyl\n\n");
        return 0;
    }

    sgen = atoi(argv[1]);
    dgen = atoi(argv[2]);
    nstream = atoi(argv[3]);
    nsamp = atoi(argv[4]);
    zgen  = atoi(argv[5]);
    sseed = atoi(argv[6]);

    //  Seed the seed generator
    if (zgen==0) {
        seed0 = sseed;
    } else {
        srandu = sseed;
    }

    //  Generate the streams
    for(i=0; i < nstream; i++) {
        sprintf(fname,"stream_%03d.dat",i);
        f = fopen(fname,"w");

        //  Seed the generators
        sseed = (zgen) ? randu() : minstd0();
        switch (dgen) {
            case 0:  CMWC_init0(sseed);     break;
            case 1:  init_genrand0(sseed);  break;
            case 2:  kiss32_init0(sseed);   break;
            default: mwseed0 |= sseed;      break;
        }
        sseed = (zgen) ? randu() : minstd0();
        switch (sgen) {
            case 0:  CMWC_init1(sseed);     break;
            case 1:  init_genrand1(sseed);  break;
            case 2:  kiss32_init1(sseed);   break;
            default: mwseed1 |= sseed;      break;
        }

        //  Generate the stream samples
        for(j=0; j<nsamp; j++) {
            switch (dgen) {
                case 0:  u = CMWC0();           break;
                case 1:  u = genrand_int320();  break;
                case 2:  u = kiss320();         break;
                default: u = middle_weyl0();    break;
            }
            switch (sgen) {
                case 0:  u ^= CMWC1();           break;
                case 1:  u ^= genrand_int321();  break;
                case 2:  u ^= kiss321();         break;
                default: u ^= middle_weyl1();    break;
            }
            fwrite((void*)&u, sizeof(uint32_t), 1, f);
        }

        fclose(f);
    }

    return 0;
}
