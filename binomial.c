#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double (*uniform)(void);

//
//  binomial_cumulative
//
double fact(int n) {
    int i;
    double x=1.0;
    for(i=1; i<=n; i++)
        x *= i;
    return x;
}

double B(int n, int k, double p) {
    double c = fact(n)/(fact(k)*fact(n-k));
    return c*pow(p,k)*pow(1.0-p, n-k);
}

int binomial_cumulative(int n, double p) {
    int i;
    double c=0.0, u,t;
    u = uniform();
    for(i=0; i<=n; i++) {
        t = B(n,i,p);
        if ((c <= u) && (u < (c+t)))
            return i;
        c += t;
    }
    return n;
}


//
//  naive coin
//
int binomial_naive_coin(int n, double a) {
    double p;
    int i,k=0;
    p = (a <= 0.5) ? a : 1.0-a;
    for(i=0; i<n; i++)
        if (uniform() <= p)
            k++;
    return (a <= 0.5) ? k : n-k;
}


//
//  recycle coin
//
int binomial_recycle_coin(int n, double a) {
    double p,u;
    int b,i,k=0;
    p = (a <= 0.5) ? a : 1.0-a;
    u = uniform();
    for(i=0; i<n; i++) {
        b = (u > (1.0-p)) ? 1 : 0;
        u = (u-(1.0-p)*b) / (p*b+(1.0-p)*(1-b));
        k += b;
    }
    return (a <= 0.5) ? k : n-k;
}


//
//  table
//
double *table = (double *)NULL;
int ntable = 0;
double ptable = 0;

int binomial_table(int n, double p) {
    int i;
    double u;

    if ((ntable==0) || (ptable==0) || (ntable != n) || (ptable != p)) {
        if (table != NULL)
            free(table);
        table = (double *)malloc((n+2)*sizeof(double));
        ntable = n;
        ptable = p;
        table[0] = 0.0;
        for(i=0; i<=n; i++)
            table[i+1] = B(n,i,p)+table[i];
    }

    u = uniform();
    for(i=0; i<n; i++)
        if ((table[i] <= u) && (u < table[i+1]))
            return i;
    return n;
}

