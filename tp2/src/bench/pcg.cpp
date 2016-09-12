/*
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For additional information about the PCG random number generation scheme,
 * including its license and other licensing options, visit
 *
 *     http://www.pcg-random.org
 */

#include <pcg.h>
#include <stdint.h>

static uint64_t pcg64_rotr(uint64_t value, unsigned int rot) {
    return (value >> rot) | (value << ((- rot) & 63));
}

void pcg64_advance(pcg64* rng, uint128_t delta) {
    uint128_t acc_mult = 1u, cur_mult = PCG64_MULTIPLIER;
    while (delta > 0) {
        if (delta & 1)
            acc_mult *= cur_mult;
        cur_mult *= cur_mult;
        delta /= 2;
    }
    rng->state = acc_mult * rng->state;
}

void pcg64_seed(pcg64* rng, uint128_t initstate) {
    rng->state = initstate | 1u;
}

uint64_t pcg64_random(pcg64* rng) {
    rng->state = rng->state * PCG64_MULTIPLIER;
    return pcg64_rotr(((uint64_t)(rng->state >> 64u)) ^ (uint64_t)rng->state, 
                      rng->state >> 122u);
}

uint64_t pcg64_bounded(pcg64* rng, uint64_t bound) {
    uint64_t threshold = -bound % bound;
    for (;;) {
        uint64_t r = pcg64_random(rng);
        if (r >= threshold)
            return r % bound;
    }
}
