#include <stdio.h>

unsigned long long seed = 0xfedcb2ed;

unsigned long random(void) {
    seed *= seed;
    seed = (seed >> 16) & 0xffffffff;
    return (unsigned long)seed;
}

int main(int argc, char *argv[]) {
    int i;
    FILE *f;
    unsigned long n;

    f = fopen(argv[2],"w");

    for(i=0; i<atoi(argv[1]); i++) {
        n = random();
        fwrite((void *)&n, 4, 1, f);
    }

    fclose(f);
    return 1;
}

