#ifndef HASH_MAP
#define HASH_MAP

#include <stdint.h>

typedef uint64_t (*HashFunc)(const void *key, uint32_t seed0, uint32_t seed1);
typedef int32_t (*Comparator)(const void *first, const void *second);
typedef void (*EntryDeleter)(void *);
typedef struct HashMap HashMap;

struct HashMapEntry {
    void *key;
    void *value;
};

typedef struct HashMapEntry HashMapEntry;

struct HashMap *create_hash_map(HashFunc hash_func, Comparator comp, EntryDeleter entry_deleter);
void release_hash_map(HashMap *);

void insert_hash_map_item(HashMap *, void *key, void *value);
void *get_hash_map_item(HashMap *, void *key);
void remove_hash_map_item(HashMap *, void *key);

#endif