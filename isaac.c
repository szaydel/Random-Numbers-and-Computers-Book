//
//  file:  isaac.c
//
//  Based on the "readable.c" version.
//
//  RTK, 14-Jan-2016
//  Last update:  14-Jan-2016
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

//  seed
uint32_t randrsl[256] = {0};

#define mix(a,b,c,d,e,f,g,h) \
{ \
   a^=b<<11; d+=a; b+=c; \
   b^=c>>2;  e+=b; c+=d; \
   c^=d<<8;  f+=c; d+=e; \
   d^=e>>16; g+=d; e+=f; \
   e^=f<<10; h+=e; f+=g; \
   f^=g>>4;  a+=f; g+=h; \
   g^=h<<8;  b+=g; h+=a; \
   h^=a>>9;  c+=h; a+=b; \
}

uint32_t isaac(uint8_t flag) {
    static uint32_t mm[256];
    static uint32_t aa=0, bb=0, cc=0;
    static randcnt=0;
    uint32_t i;

    if (flag) {
        //  initialize with seed first
        uint32_t a,b,c,d,e,f,g,h;

        aa=bb=cc=0;
        a=b=c=d=e=f=g=h=0x9e3779b9;  // the golden ratio

        for (i=0; i<4; ++i)          // scramble it
            mix(a,b,c,d,e,f,g,h);

        for (i=0; i<256; i+=8) {   // fill in mm[] with messy stuff
            a+=randrsl[i  ]; b+=randrsl[i+1]; c+=randrsl[i+2]; d+=randrsl[i+3];
            e+=randrsl[i+4]; f+=randrsl[i+5]; g+=randrsl[i+6]; h+=randrsl[i+7];
            mix(a,b,c,d,e,f,g,h);
            mm[i  ]=a; mm[i+1]=b; mm[i+2]=c; mm[i+3]=d;
            mm[i+4]=e; mm[i+5]=f; mm[i+6]=g; mm[i+7]=h;
        }
        for (i=0; i<256; i+=8) {
            a+=mm[i  ]; b+=mm[i+1]; c+=mm[i+2]; d+=mm[i+3];
            e+=mm[i+4]; f+=mm[i+5]; g+=mm[i+6]; h+=mm[i+7];
            mix(a,b,c,d,e,f,g,h);
            mm[i  ]=a; mm[i+1]=b; mm[i+2]=c; mm[i+3]=d;
            mm[i+4]=e; mm[i+5]=f; mm[i+6]=g; mm[i+7]=h;
        }
        randcnt = 256;
    }
    
    //  Update the state
    if (randcnt == 256) {
        // update
        uint32_t x,y;

        cc = cc + 1;    // cc just gets incremented once per 256 results
        bb = bb + cc;   // then combined with bb

        for (i=0; i<256; ++i) {
            x = mm[i];
            switch (i%4) {
                case 0: aa = aa^(aa<<13); break;
                case 1: aa = aa^(aa>>6); break;
                case 2: aa = aa^(aa<<2); break;
                case 3: aa = aa^(aa>>16); break;
            }
            aa              = mm[(i+128)%256] + aa;
            mm[i]      = y  = mm[(x>>2)%256] + aa + bb;
            randrsl[i] = bb = mm[(y>>10)%256] + x;
        }
        randcnt = 0;
    }

    //  Return the next value
    return randrsl[randcnt++];
}

