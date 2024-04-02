#include <sdk/map.h>

static const uint _nPrimes  = 15;
static const uint _Primes[] =
{
  23,     53,     97,     193,     389,
  769,    1543,   3079,   6151,    12289,
  24593,  49157,  98317,  196613,  393241
};

static void rehash(map_t *map, entry_t **new_buckets, uint new_capacity)
{
  for (uint i = 0; i < map->capacity; i++)
  {
    entry_t *it;

    for (it = map->buckets[i]; it; it = it->next)
    {
      u64 index = it->hash % new_capacity;
    
      it->next = new_buckets[index];
      new_buckets[index] = it;
    }
  }
}

static uint next_prime(uint current)
{
  for (uint i = 0; i < _nPrimes; i++)
  {
    if (current < _Primes[i])
      return _Primes[i];
  }

  return 0;
}

static bool grow(map_t *map)
{
  uint new_capacity = next_prime(map->capacity);
  assert(new_capacity);

  if (!new_capacity)
    return false;

  entry_t **new_buckets = allocate(map->allocator, new_capacity * sizeof(entry_t *));
  assert(new_buckets);

  if (!new_buckets)
    return false;

  memset(new_buckets, 0, new_capacity * sizeof(entry_t *));

  rehash(map, new_buckets, new_capacity);
  
  deallocate(map->allocator, map->buckets);

  map->buckets  = new_buckets;
  map->capacity = new_capacity;
  
  return true;
}

map_t *map_create(uint reserve, hash_fn fn, allocator_t *allocator)
{
  assert(fn);

  map_t *map;

  map = allocate(allocator, sizeof(map_t));
  assert(map);

  if (!map)
    return NULL;

  map->hash      = fn;
  map->allocator = allocator;
  map->entries   = 0;
  map->capacity  = reserve;

  map->buckets = allocate(allocator, map->capacity * sizeof(entry_t *));
  assert(map->buckets);

  memset(map->buckets, 0, map->capacity * sizeof(entry_t *));

  if (!map->buckets)
  {
    deallocate(allocator, map);
    return NULL;
  }

  return map;
}

void map_delete(map_t *map)
{
  assert(map && map->buckets);

  for (uint i = 0; i < map->capacity; i++)
  {
    for (entry_t *it = map->buckets[i]; it; it = it->next)
      deallocate(map->allocator, it);
  }

  deallocate(map->allocator, map->buckets);
  deallocate(map->allocator, map);
}

entry_t *map_find(map_t *map, void *key, u64 size)
{
  assert(map && map->buckets);

  u64 hash  = map->hash(key, size);
  u64 index = hash % map->capacity;

  entry_t *it;
  for (it = map->buckets[index]; it; it = it->next)
  {
    if (!it)
      return NULL;
  
    if (it->hash != hash && it->size != size)
      continue;

    if (!memcmp(it->key, key, size))
      return it;
  }

  return NULL;
}

void *map_set(map_t *map, void *key, u64 size, void *value)
{
  assert(map && map->buckets);

  entry_t *entry = map_find(map, key, size);

  if (!entry)
  {
    u64 hash  = map->hash(key, size);
    u64 index = hash % map->capacity;

    if (map->entries == map->capacity)
      if (!grow(map))
        return NULL;

    entry         = allocate(map->allocator, sizeof(entry_t));
    entry->key    = key;
    entry->hash   = hash;
    entry->size   = size;
    entry->next   = map->buckets[index];
  
    map->buckets[index] = entry;
    map->entries++;
  }

  entry->value = value;
  
  return entry;
}

void *map_get(map_t *map, void *key, u64 size)
{
  assert(map && map->buckets);

  entry_t *entry = map_find(map, key, size);

  if (!entry)
    return NULL;
  
  return entry->value;
}
