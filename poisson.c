//
//  file:  poisson.c
//
//  Code to sample from a poisson distribution given
//  uniform U[0,1) inputs.
//
//  RTK, 27-Sep-2017
//  Last update:  29-Sep-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//  Assign to a uniform generator
double (*uniform)(void);

//  multiplication - Knuth - only suitable for lambda < 10
int poisson_mult(double l) {
    double m=exp(-l), p;
    int k=0;

    p = uniform();
    while (p >= m) {
        k++;
        p *= uniform();
    }
    return k;
}

//  inversion sequential search - Devroye - p 505
int poisson_inv(double l) {
    double u=uniform(), s, p=exp(-l);
    int k=0;

    s = p;
    while (u > s) {
        k++;
        p *= l/k;
        s += p;
    }
    return k;
}

//  up/down - Kemp - KEMPOIS - from FORTRAN p 15
int poisson_kemp_downward(double l, double u, double q, int m) {
    int i, mmin;
    if (u < q) 
        return m;
    mmin=m-1;
    for(i=0; i<mmin; i++) { 
        u=u-q;
        q=(m-i)*q/l;
        if (u < q) 
            return mmin-i;
    }
    return 0;
}

int poisson_kemp_upward(double l, double u, double q, int m) {
    int i;
    u=1.0-u;
    for(i=m+1; i<=m+m+30; i++) {
        q=q*l/(double)i;
        if (u < q) 
            return i;
       u = u-q;
    }
}

int poisson_kemp(double l) {
    int i,mmin;
    double c,c1,c2,c3,d,d1,d2,d3,d4,e,e1,e2,f1,f2,f3,h;
    double qq,u,a,aa,g,m,p,q,rm,sp,sq;

    d=1./2.; e=2./3.; c1=1./12.; c2=1./24; c3=293./8640.;
    d1=23./270.; d2=5./56.; d3=30557./649152.; e1=1./4.;
    e2=1./18.; f1=1./6.; f2=3./20.; f3=1./20.; h=7./6.;
    c=0.3989423; d4=34533608./317640015.;

    u = uniform();
    m = (int)(l+d);
    rm = 1.0/(double)m;
    a = l-m;
    g = c/sqrt((double)m);
    aa = a*a;
    p = g*(1.0-c1/(m+c2+c3*rm));
    q = p*(1.0-d*aa/(m+a*(e+a*(e1-e2*rm))));

    if (u < d)
        return poisson_kemp_downward(l,u,q,m);
    if (u > (d+h*g))
        return poisson_kemp_upward(l,u,q,m);

    sp = d+g*(e-d1/(m+d2+d3/(m+d4)));
    sq = sp-a*p*(1.0-f1*aa/(m+a*(d+a*(f2-f3*rm))));
    if (u > sq) 
        return poisson_kemp_upward(l,u,q,m);
    return poisson_kemp_downward(l,u,q,m);
}


#if 0
int poisson_kemp_orig(double l) {
    int i,mmin;
    double c,c1,c2,c3,d,d1,d2,d3,d4,e,e1,e2,f1,f2,f3,h;
    double qq,u;
    static double l1=0.0, l2=0.0;
    static double a,aa,g,m,p,q,rm,sp,sq;

    // parameters
    d=1./2.; e=2./3.; c1=1./12.; c2=1./24; c3=293./8640.;
    d1=23./270.; d2=5./56.; d3=30557./649152.; e1=1./4.;
    e2=1./18.; f1=1./6.; f2=3./20.; f3=1./20.; h=7./6.;
    c=0.3989423; d4=34533608./317640015.;

    // ugly port from old FORTRAN follows!
    u = uniform();

    if (l == l2) goto L6;
    if (l == l1) goto L5;

    l1 = l;
    m = (int)(l+d);
    rm = 1.0/(double)m;
    a = l-m;
    g = c/sqrt((double)m);
    aa = a*a;
    p = g*(1.0-c1/(m+c2+c3*rm));
    q = p*(1.0-d*aa/(m+a*(e+a*(e1-e2*rm))));

L5: if (u < d) goto L10;
    if (u > (d+h*g)) goto L20;
    l2=l;
    sp = d+g*(e-d1/(m+d2+d3/(m+d4)));
    sq = sp-a*p*(1.0-f1*aa/(m+a*(d+a*(f2-f3*rm))));

L6: if (u > sq) goto L20;

//  downwards search
L10:if (u < q) return m;
    qq=q;
    mmin=m-1;
    for(i=0; i<mmin; i++) { 
        u=u-qq;
        qq=(m-i)*qq/l;
        if (u < qq) return mmin-i;
    }
    return 0;

//  upwards search
L20:u=1.0-u;
    qq=q;
    for(i=m+1; i<=m+m+30; i++) {
       qq=qq*l/(double)i;
       if (u < qq) return i;
       u = u-qq;
    }
}
#endif

