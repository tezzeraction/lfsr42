#ifndef _LFSR42_H_
#define _LFSR42_H_

#include <stdint.h>     // uint64_t, etc

#define _NL "\n"

#define NN 42

typedef uint64_t vector_t;
typedef vector_t matrix_t[NN];

void lfsr42_init(void);
uint64_t lfsr42_galois(uint64_t shift, uint64_t initial_state);

// GCC trick to use default value 1ULL for initial_state agrument
#define lfsr42(shift, ...) lfsr42_galois(shift, (1ULL, ##__VA_ARGS__))

void show_v(vector_t vv);
void show_w(matrix_t mm);

#endif
