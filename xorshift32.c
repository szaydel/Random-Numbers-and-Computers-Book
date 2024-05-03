#define A 13
#define B 17
#define C  5

unsigned long s32 = 2463534242UL;

unsigned long xorshift32(void) {
    s32 ^= (s32<<A);
    s32 ^= (s32>>B);
    s32 ^= (s32<<C);
    return s32;
}

