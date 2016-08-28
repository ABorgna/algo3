#pragma once

#include <stdint.h>
#include <inttypes.h>

typedef __uint128_t uint128_t;

typedef struct {
    uint128_t state;
} pcg64;

#define U64(high,low) ((((uint64_t)high) << 32) + low)
#define U128(high,low) ((((uint128_t)high) << 64) + low)
#define PCG64_MULTIPLIER U128(2549297995355413924ULL,4865540595714422341ULL)
#define PCG64_INITIALIZER {U128(0x0000000000000000ULL, 0xcafef00dd15ea5e5ULL)}

void pcg64_advance(pcg64* rng, uint128_t delta);
void pcg64_seed(pcg64* rng, uint128_t initstate);
uint64_t pcg64_random(pcg64* rng);
uint64_t pcg64_bounded(pcg64* rng, uint64_t bound); 

