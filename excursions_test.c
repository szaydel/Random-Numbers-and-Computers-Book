//
//  file: excursions_test.c
//
//  An implementation of the NIST random excursions test
//
//  RTK, 13-Oct-2017
//  Last update:  13-Oct-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

#define N 10000000

int8_t B[N];
int16_t S[N];
int T[8][6];
int V[] = {-4,-3,-2,-1,1,2,3,4};
double P[][6] ={{0.8750,0.0156,0.0137,0.0120,0.0105,0.0733},
               {0.8333,0.0278,0.0231,0.0193,0.0161,0.0804},
               {0.7500,0.0625,0.0469,0.0352,0.0264,0.0791},
               {0.5000,0.2500,0.1250,0.0625,0.0312,0.0312},
               {0.5000,0.2500,0.1250,0.0625,0.0312,0.0312},
               {0.7500,0.0625,0.0469,0.0352,0.0264,0.0791},
               {0.8333,0.0278,0.0231,0.0193,0.0161,0.0804},
               {0.8750,0.0156,0.0137,0.0120,0.0105,0.0733}};

double igf(double s, double z) {
    double sc=(1.0/s), sum=1.0;
    double num=1.0, denom=1.0;
    int i;

    if (z < 0.0)
        return 0.0;

    sc *= pow(z,s);
    sc *= exp(-z);

    for(i=0; i<60; i++) {
        num *= z;
        s++;
        denom *= s;
        sum += (num/denom);
    }

    return sum*sc;
}


double chisq_pval(double cv, int dof) {
    double k = (double)dof * 0.5;
    double x = cv * 0.5;

    if ((cv < 0) || (dof < 1))
        return 0.0;
    if (dof == 2)
        return exp(-x);

    return igf(k,x)/tgamma(k);
}


int next(FILE *g) {
    uint8_t b8;
    if (fread((void*)&b8, sizeof(uint8_t), 1, g) == 0)
        return -1;
    return (int)b8;
}


void buildS(char *src) {
    int v,i,j,k=0,b,z,w=0;
    FILE *g;

    //  load B - must have at least N bits
    g = fopen(src,"r");
    for(i=0; i<(N/8); i++) {
        b = next(g);        
        if (b == -1) {
            printf("The source file must have at least 1,000,000 bits\n");
            exit(0);
        }
        B[k++] = (b & 128) ? 1 : -1;
        B[k++] = (b &  64) ? 1 : -1;
        B[k++] = (b &  32) ? 1 : -1;
        B[k++] = (b &  16) ? 1 : -1;
        B[k++] = (b &   8) ? 1 : -1;
        B[k++] = (b &   4) ? 1 : -1;
        B[k++] = (b &   2) ? 1 : -1;
        B[k++] = (b &   1) ? 1 : -1;
    }
    fclose(g);

    //  create S
    k=0;
    S[0] = B[k++];
    for(i=1; i<N; i++) {
        S[i] = B[k++] + S[i-1];
    }

    //  fill in T table
    for(j=0; j<8; j++) {
        v = V[j];
        for(k=0; k<6; k++) {
            b=z=0;
            for(i=0; i<N; i++) {
                if (S[i] == v) b++;    
                if (S[i] == 0) {
                    if (b==k) z++;
                    if ((k==5) && (b>5)) z++;
                    b=0;
                    w++;
                }
            }
            T[j][k] = z;
        }
    }

    //  Too few cycles?
    if (w<500) {
        printf("Too few cycles, test fails\n");
        exit(1);
    }
}


void excursions(void) {
    double n,v,p;
    int i,j;

    for(i=0; i<8; i++) {
        v = 0.0;
        n=0;
        for(j=0; j<6; j++)
            n += T[i][j];
        for(j=0; j<6; j++) {
            v += (T[i][j]-n*P[i][j])*(T[i][j]-n*P[i][j])/(n*P[i][j]);
        }
        p = chisq_pval(v,5);
        printf("v=%2d, chisq=%0.4f, p=%0.7f\n",V[i],v,p);
    }
}


int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("\nrandom_excursions_test <source>\n\n");
        printf("  <source> - source binary file (bits, not FP)\n\n");
        return 0;
    }

    buildS(argv[1]);
    excursions();
}


