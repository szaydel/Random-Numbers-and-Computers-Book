#include <Random123/threefry.h>
#include <Random123/philox.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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


//  threefry
threefry2x64_key_t threefry64key = {{0,0}};

void threefry64_init(uint64_t seed) {
    threefry64key.v[0] = seed;
}

uint64_t threefry64(void) {
    static c=2,i=0;
    static threefry2x64_ctr_t r;
    uint64_t ans;
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


void main(int argc, char *argv[]) {
    int i;
    uint32_t seed;

    seed = (uint32_t)atoi(argv[1]);

    philox32_init(seed);
    for(i=0; i<10; i++)
        printf("%d : %lu\n", i, (unsigned long)philox32());
    printf("\n");
    threefry64_init(seed);
    for(i=0; i<10; i++)
        printf("%d : %llu\n", i, (unsigned long long)threefry64());
}

