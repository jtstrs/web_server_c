#ifndef MURMUR_HASH_3
#define MURMUR_HASH_3

#include <stdint.h>

uint64_t mm_86_128(const void *key, const int32_t len, uint32_t seed);

#endif