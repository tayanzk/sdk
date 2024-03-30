#include "arena.h"

typedef struct region_t
{
  struct region_t *next;
  unsigned count, capacity;

  uintptr_t data[];
} region_t;

static const uint _Default = (12 * 1024);

region_t *region_create(size_t capacity)
{
  size_t size = sizeof(region_t) + sizeof(uintptr_t) * capacity;

  region_t *region = malloc(size);
  assert(region);

  if (!region)
    return NULL;

  region->next     = NULL;
  region->count    = 0;
  region->capacity = capacity;
  
  return region;
}

void region_delete(region_t *region)
{
  free(region);
}

void *arena_allocate(arena_t *arena, size_t size)
{
  size_t new = (size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

  if (arena->end == NULL)
  {
    assert(arena->begin == NULL);
    
    size_t capacity = _Default;
    
    if (capacity < new)
      capacity = new;
    
    arena->end   = region_create(capacity);
    arena->begin = arena->end;
  }

  while (arena->end->count + new > arena->end->capacity && arena->end->next != NULL)
    arena->end = arena->end->next;

  if (arena->end->count + new > arena->end->capacity)
  {
    assert(arena->end->next == NULL);
    
    size_t capacity = _Default;

    if (capacity < new)
      capacity = new;
    
    arena->end->next = region_create(capacity);
    arena->end       = arena->end->next;
  }

  void *result = &arena->end->data[arena->end->count];
  arena->end->count += new;

  return result;
}

void *arena_reallocate(arena_t *arena, void *ptr, size_t old, size_t new)
{
  if (new <= old)
    return ptr;

  char *cnew = arena_allocate(arena, new);
  char *cptr = ptr;

  for (size_t i = 0; i < old; i++)
    cnew[i] = cptr[i];

  return new;
}

void arena_reset(arena_t *arena)
{
  region_t *it = arena->begin;

  while (it)
  {
    it->count = 0;
    it = it->next;
  }

  arena->end = arena->begin;
}

void arena_free(arena_t *arena)
{
  region_t *region = arena->begin;

  while (region)
  {
    region_t *it = region;
    region = region->next;

    region_delete(it);
  }

  arena->begin = NULL;
  arena->end   = NULL;
}
