/*
    27.04.2023
    Author: Dmytro Tarasiuk (tadmytro@gmail.com)
    Exam Task from OzzyLogic: LFSR 42 bits realization.
*/

#include "lfsr42.h"

#include <stdio.h>      // printf(), etc
#include <stdint.h>     // uint64_t, etc

/* The following agreement are used:
   1) identifier prefix/suffix "w" means square matrix, or vector
        of vectors, "vv" -> "w",
   2) identifier prefix/suffix "v" means one line/column matrix, or
        vector. 
*/

matrix_t g_cm =
// Companion matrix (cm) for Galois LFSR42 with the following feedback
// polinomial:
//  x^42
// {1 1 1 1 0 1 1 1 0 0 1 0 0 0 0 0 1 1 1 1 0                      x^1
//                         1 1 0 0 1 1 1 0 0 0 1 0 1 0 1 0 0 0 0 0 0};
{ 0b000000000000000000000000000000000000000010ULL
, 0b000000000000000000000000000000000000000100ULL
, 0b000000000000000000000000000000000000001000ULL
, 0b000000000000000000000000000000000000010000ULL
, 0b000000000000000000000000000000000000100000ULL
, 0b000000000000000000000000000000000001000000ULL
, 0b000000000000000000000000000000000010000001ULL
, 0b000000000000000000000000000000000100000000ULL
, 0b000000000000000000000000000000001000000001ULL
, 0b000000000000000000000000000000010000000000ULL
, 0b000000000000000000000000000000100000000001ULL
, 0b000000000000000000000000000001000000000000ULL
, 0b000000000000000000000000000010000000000000ULL
, 0b000000000000000000000000000100000000000000ULL
, 0b000000000000000000000000001000000000000001ULL
, 0b000000000000000000000000010000000000000001ULL
, 0b000000000000000000000000100000000000000001ULL
, 0b000000000000000000000001000000000000000000ULL
, 0b000000000000000000000010000000000000000000ULL
, 0b000000000000000000000100000000000000000001ULL
, 0b000000000000000000001000000000000000000001ULL
, 0b000000000000000000010000000000000000000000ULL
, 0b000000000000000000100000000000000000000001ULL
, 0b000000000000000001000000000000000000000001ULL
, 0b000000000000000010000000000000000000000001ULL
, 0b000000000000000100000000000000000000000001ULL
, 0b000000000000001000000000000000000000000000ULL
, 0b000000000000010000000000000000000000000000ULL
, 0b000000000000100000000000000000000000000000ULL
, 0b000000000001000000000000000000000000000000ULL
, 0b000000000010000000000000000000000000000000ULL
, 0b000000000100000000000000000000000000000001ULL
, 0b000000001000000000000000000000000000000000ULL
, 0b000000010000000000000000000000000000000000ULL
, 0b000000100000000000000000000000000000000001ULL
, 0b000001000000000000000000000000000000000001ULL
, 0b000010000000000000000000000000000000000001ULL
, 0b000100000000000000000000000000000000000000ULL
, 0b001000000000000000000000000000000000000001ULL
, 0b010000000000000000000000000000000000000001ULL
, 0b100000000000000000000000000000000000000001ULL
, 0b000000000000000000000000000000000000000001ULL};

// For each binary digit in shift number corresponds square matrix,
// which is calculated with help lfsr42_init()
matrix_t g_pow2w[NN];

static void copy_w(matrix_t from, matrix_t to)
{
    for (unsigned i = 0; i < NN; ++i)
        to[i] = from[i];
}

static void w_mult_w(matrix_t ma, matrix_t mb, matrix_t mr)
{
    unsigned i, j, k;
    register vector_t jm, km;
    for (i = 0; i < NN; ++i)
        for (j = 0, jm = 1; j < NN; ++j, jm <<= 1) {
            int b = 0;
            for (k = 0, km = 1; k < NN; ++k, km <<= 1)
                // mr[i,j] = mr[i,j] + ma[i,k] * mb[k,j]
                b ^= ((ma[i] & km) != 0) & ((mb[k] & jm) != 0);
            if (b)
                mr[i] |= jm;  // set bit
            else
                mr[i] &= ~jm; // clear bit
        }
}

void lfsr42_init(void)
{
    copy_w(g_cm, g_pow2w[0]);
    for (unsigned i = 0; i < NN-1; ++i)
        w_mult_w(g_pow2w[i], g_pow2w[i], g_pow2w[i+1]);
}

static vector_t w_mult_v(matrix_t ma, vector_t vb)
{
    unsigned i, j;
    vector_t vr = 0;
    vector_t im, jm;
    for (i = 0, im = 1; i < NN; ++i, im <<= 1) {
        int b = 0;
        for (j = 0, jm = 1; j < NN; ++j, jm <<= 1)
            // vr[i] = vr[i] + ma[i,j] * vb[j]
            b ^= ((ma[i] & jm) != 0) & ((vb & jm) != 0);
        if (b)
            vr |= im;  // set bit
        else
            vr &= ~im; // clear bit 
    }
    return vr;
}

void show_v(vector_t vv)
{
    char buf[sizeof("1234567890 1234567890 1234567890 1234567890 12")];
    unsigned i = 0, j = 0;
    for (vector_t mask = 1; mask <= 0x20000000000; mask <<= 1) {
        if (0 != (vv & mask))
            buf[i] = '1';
        else
            buf[i] = '0';
        i++;
        if (0 == (++j % 8))
            buf[i++] = ' ';
    }
    buf[i] = 0;
    printf("%s"_NL, buf);
}

void show_w(matrix_t mm)
{
    unsigned i;
    for (i = 0; i < NN; ++i)
        show_v(mm[i]);
}

uint64_t lfsr42_galois(uint64_t shift, uint64_t initial_state)
{
    if (0 == shift)
        return initial_state;

    matrix_t mr, mr0;
    register uint64_t mask;
    register unsigned i;
    int f = 1;
    for (i = 0, mask = 1; i < NN; ++i, mask <<= 1)
        if (shift & mask) {
            if (f) {
                copy_w(g_pow2w[i], mr);
                f = 0;
            }
            else {
                w_mult_w(mr, g_pow2w[i], mr0);
                copy_w(mr0, mr);
            }
        }
    return w_mult_v(mr, initial_state);
}
