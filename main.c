/*
    27.04.2023
    Author: Dmytro Tarasiuk (tadmytro@gmail.com)
    Exam Task from OzzyLogic: LFSR 42 bits realization.
*/

#include <stdio.h>      // printf(), etc
#include <stdlib.h>     // EXIT_SUCCESS, etc
#include <time.h>       // to estimate the execution time 

#include "lfsr42.h"

int main(void)
{
    lfsr42_init();

    uint64_t checkdata[] = {0, 1, 2, 3, 4, 100, 663125524836, 663124934834, 663125229746, 663125524658};

    for (int i = 0; i < sizeof(checkdata) / sizeof(uint64_t); ++i) {
        printf("Shift %13lu: ", checkdata[i]);
        show_v(lfsr42(checkdata[i]));
    }

    printf("Time measurement. Please wait..."_NL);

#   define ITERATIONS 5000
#   define BEGIN_FROM 663125524836ULL
    clock_t start, end;
    start = clock();
    for (uint64_t n = BEGIN_FROM; n < BEGIN_FROM + ITERATIONS; ++n)
        lfsr42(n);
    end = clock();
    double time_used = ((double) (end - start)) / CLOCKS_PER_SEC / ITERATIONS * 1000;
    printf("Used time is %f ms"_NL, time_used);
#   undef ITERATIONS
#   undef BEGIN_FROM

    return EXIT_SUCCESS;
}
