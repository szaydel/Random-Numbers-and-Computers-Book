#include <stdio.h>

unsigned int A[] = {113,331,991,3001,9011};
unsigned int B[] = {31,97,313,919,3011};
unsigned int C[] = {23,61,181,419,1217};
unsigned int a,b,c;

unsigned int AA(unsigned int j) {  a = (a+1) % A[j]; return a; }
unsigned int BB(unsigned int j) {  b = (b+1) % B[j]; return b; }
unsigned int CC(unsigned int j) {  c = (c+1) % C[j]; return c; }

void main() {
    unsigned int j;
    unsigned long long i;

    for(j=0; j<5; j++) {
        i=a=b=c=0;
        do {
            AA(j); BB(j); CC(j);
            i++;
            if ((a==0) && (b==0) && (c==0))
                break;
        } while (1);
        printf("(%u,%u,%u) = %llu\n", A[j],B[j],C[j],i);
    }
}

