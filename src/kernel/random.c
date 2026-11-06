#include "random.h"

#define MULTIPLIER 1103515245ULL
#define INCREMENT 12345ULL

u32 seed = 999;

void rands(u32 _seed) {
    seed = _seed & 0x7FFFFFFF;
}

u32 rand_u32(void) {
    seed = ((u64) (MULTIPLIER * seed) + INCREMENT) & 0x7FFFFFFF;
    return seed;
}