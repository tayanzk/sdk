#ifndef _ARENA_h
#define _ARENA_h 1

#include <sdk/sdk.h>

typedef struct arena_t
{
  uint total, consumed;
  struct region_t *begin, *end;
} arena_t;

void *arena_allocate(arena_t *arena, size_t size);
void *arena_reallocate(arena_t *arena, void *ptr, size_t old, size_t new);
void *arena_deallocate(arena_t *arena, void *ptr);

void arena_reset(arena_t *arena);
void arena_delete(arena_t *arena);

#endif
