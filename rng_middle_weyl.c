/*
 * This is a wrapping of the Middle Weyl RNG
 */

 // 1. Create a new .c file following rng_empty_random.c template, that defines the 
 //    user-defined RNG.
 // 2. Add the RNG reference to the bottom of dieharder.h.
 // 3. Add the RNG reference to add_ui_rngs.c.
 // 3.5. Add "add_ui_rngs()" after "dieharder_rng_types()" in dieharder.c
 // 4. Add the new .c file to Makefile.am.
 // 5. "make" the program:
 //      autoreconf
 //      ./configure
 //      make
 // 6. Run the program ./dieharder -g -l and confirm a new generator 600 is listed.
 // 7. Run the program ./dieharder -a -g 600.

#include "dieharder.h"
#include <stdint.h>

uint64_t mw_seed = 0xb5ad4ece00000000;

unsigned long int mw_random(void) {
    static uint64_t x=0, w=0;
    x *= x;
    w += mw_seed;
    x += w;
    x = (x >> 32) | (x << 32);
    return (unsigned long int)x;
}

static unsigned long int mw_random_get (void *vstate) {
    return mw_random();
}

static double mw_random_get_double (void *vstate) {
    return (double)mw_random() / 4294967296.0;
}

static void mw_random_set (void *vstate, unsigned long int s) {
    mw_seed |= (uint64_t)seed;
}

typedef struct
  {
  }
mw_random_state_t;

static const gsl_rng_type mw_random_type =
{"middle_weyl",			/* name */
 4294967295UL,	/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (mw_random_state_t),
 &mw_random_set,
 &mw_random_get,
 &mw_random_get_double};

const gsl_rng_type *gsl_rng_mw_random = &mw_random_type;

