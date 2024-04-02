#ifndef _MAP_h 
#define _MAP_h 1

#include <sdk/sdk.h>
#include <sdk/alloc.h>

typedef u64 (*hash_fn) (char *, u64);

typedef struct entry_t
{
  void *key;
  void *value;
  
  u64 hash;
  u64 size;

  struct entry_t *next;
} entry_t;

typedef struct map_t
{
  hash_fn hash;
  allocator_t *allocator;
  
  entry_t **buckets;
  uint entries, capacity;
} map_t;

map_t *map_create(uint reserve, hash_fn fn, allocator_t *allocator);
void   map_delete(map_t *map);

entry_t *map_find(map_t *map, void *key, u64 size);

void *map_set(map_t *map, void *key, u64 size, void *value);
void *map_get(map_t *map, void *key, u64 size);

#endif
