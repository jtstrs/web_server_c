#include "hash_map.h"
#include "list.h"

#include <stdint.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR 0.75

HashMap *create_hash_map(HashFunc hash_func, Comparator comp, EntryDeleter entry_deleter) {

    if (!hash_func || !comp || !entry_deleter) {
        return NULL;
    }

    HashMap *instance = (HashMap *) malloc(sizeof(HashMap));

    if (!instance) {
        return NULL;
    }

    instance->buckets_count = INITIAL_CAPACITY;
    instance->buckets = malloc(sizeof(Bucket) * instance->buckets_count);

    if (!instance->buckets) {
        free(instance);
        return NULL;
    }

    for (int32_t bucket_index = 0; bucket_index < instance->buckets_count; ++bucket_index) {
        List *entries_list = create_list(entry_deleter);

        if (!entries_list) {
            free(instance->buckets);
            free(instance);
        }

        instance->buckets[bucket_index].entries = entries_list;
    }

    instance->hash_func = hash_func;
    instance->compare_func = comp;
    instance->entry_deleter = entry_deleter;

    return instance;
}

void release_hash_map(HashMap *hash_map) {
    if (!hash_map) {
        return;
    }

    for (int32_t bucket_index = 0; bucket_index < hash_map->buckets_count; ++bucket_index) {
        release_list(hash_map->buckets[bucket_index].entries);
    }

    free(hash_map->buckets);
    free(hash_map);
}

int32_t get_entry_position(HashMap *hash_map, void *key, int32_t seed0, int32_t seed1) {
    const uint64_t hash = hash_map->hash_func(key, 0, 0);
    return hash % hash_map->buckets_count;
}

size_t find_entry_in_bucket(List *bucket_entries, void *key) {
    size_t bucket_size = get_list_size(bucket_entries);

    for (size_t item_index = 0; item_index < bucket_size; ++item_index) {
        HashMapEntry *entry = (HashMapEntry *) get_list_item(bucket_entries, item_index);
        if (entry->key == key) {
            return item_index;
        }
    }
    return -1;
}

void insert_hash_map_item(HashMap *hash_map, void *key, void *value) {
    if (!hash_map) {
        return;
    }

    HashMapEntry *entry = (HashMapEntry *) malloc(sizeof(HashMapEntry));

    if (!entry) {
        return;
    }

    entry->key = key;
    entry->value = value;

    const size_t bucket_index = get_entry_position(hash_map, key, 0, 0);
    Bucket *bucket = &hash_map->buckets[bucket_index];

    const size_t item_index = find_entry_in_bucket(bucket->entries, key);

    if (item_index == -1) {
        push_list_item(key, entry);
    } else {
        remove_list_item(bucket->entries, item_index);
        push_list_item(key, entry);
    }

    hash_map->size++;
}


void *get_hash_map_item(HashMap *hash_map, void *key) {
    if (!hash_map || !key) {
        return NULL;
    }
    const int32_t insert_position = get_entry_position(hash_map, key, 0, 0);
    const int32_t bucket_list_size = get_list_size(hash_map->buckets[insert_position].entries);

    for (int32_t entry_index = 0; entry_index < bucket_list_size; ++entry_index) {
        HashMapEntry *entry = (HashMapEntry *) get_list_item(hash_map->buckets[insert_position].entries, entry_index);

        if (hash_map->compare_func(entry->key, key) == 0) {
            return entry->value;
        }
    }

    return NULL;
}

void remove_hash_map_item(HashMap *hash_map, void *key) {
    if (!hash_map) {
        return;
    }

    const size_t bucket_index = get_entry_position(hash_map, key, 0, 0);
    Bucket *bucket = &hash_map->buckets[bucket_index];

    const size_t item_index = find_entry_in_bucket(bucket->entries, key);

    if (item_index != -1) {
        remove_list_item(bucket->entries, item_index);
    }
}

int32_t get_hash_map_item_count(HashMap *hash_map) {
    if (!hash_map) {
        return 0;
    }
    return hash_map->size;
}