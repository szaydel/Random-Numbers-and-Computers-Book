//
//  file:  monty.c
//
//  Play the Monty Hall Dilemma using a file of
//  random bytes.
//
//  RTK, 21-Jun-2017
//  Last update:  21-Jun-2017
//
///////////////////////////////////////////////////////////////

#include <stdio.h>

unsigned char randint(unsigned char b, unsigned char mx) {
    return (int)(mx*((float)b/256.0));
}


int main(int argc, char *argv[]) {
    FILE *f;
    unsigned char b, win, guess;
    int i, n, niter, win0, win1;

    if (argc == 1) {
        printf("\nmonty <random>\n\n");
        printf("  <random>  -  a file of random bytes\n\n");
        return 0;
    }

    //  Set up the problem - track iterations, wins and losses
    win0 = win1 = 0;

    //  Open the file of random bytes and get the size
    f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);

    //  Number of iterations
    niter = n / 2;

    //  Run the simulation
    for (i=0; i < niter; i++) {
        //  Get the next byte and split into winning door, initial guess, Monty's door, and switch
        b = fgetc(f);
        win = randint(b,3);
        b = fgetc(f);
        guess = randint(b,3);

        //  Will we win if we don't switch doors?
        if (win == guess) win0++;

        //  Let's switch doors
        switch (guess) {
            case 0:
                if (win == 0) guess = 1;  // door 1 or 2
                if (win == 1) guess = 1;  // door 2
                if (win == 2) guess = 2;  // door 1
                break;
            case 1:
                if (win == 0) guess = 0;  // door 2
                if (win == 1) guess = 0;  // door 0 or 2
                if (win == 2) guess = 2;  // door 0
                break;
            case 2:
                if (win == 0) guess = 0;  // door 1
                if (win == 1) guess = 1;  // door 0
                if (win == 2) guess = 0;  // door 0 or 1
                break;
        }

        //  Do we win now?
        if (win == guess) win1++;
    }

    fclose(f);

    //  Report the results
    printf("\nResults after %d simulations:\n\n", niter);
    printf("  Wins on initial guess  = %d (%0.4f %%)\n", win0, 100.0*(float)win0/(float)niter);
    printf("  Wins on changing guess = %d (%0.4f %%)\n", win1, 100.0*(float)win1/(float)niter);
    printf("\n");

    return 0;
}
