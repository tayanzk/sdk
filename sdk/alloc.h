#ifndef _ALLOC_h
#define _ALLOC_h 1

#include <sdk/sdk.h>

typedef void *(*alloc_fn)   (void *user, size_t size);
typedef void *(*realloc_fn) (void *user, void *ptr, size_t old, size_t new);
typedef void *(*dealloc_fn) (void *user, void *ptr);

typedef struct allocator_t
{
  void *user;

  alloc_fn   allocate;
  realloc_fn reallocate;
  dealloc_fn deallocate;
} allocator_t;

static inline void *allocate(allocator_t *allocator, size_t size)
{
  if (!allocator)
    return malloc(size);

  if (allocator->allocate)
    return allocator->allocate(allocator->user, size);

  return NULL;
}

static inline void *reallocate(allocator_t *allocator, void *ptr, size_t old, size_t new)
{
  if (!allocator)
    return realloc(ptr, new);

  if (allocator->reallocate)
    return allocator->reallocate(allocator->user, ptr, old, new);

  return NULL;
}

static inline void *deallocate(allocator_t *allocator, void *ptr)
{
  if (!allocator)
  {
    free(ptr);
    return NULL;
  }

  if (allocator->deallocate)
    return allocator->deallocate(allocator->user, ptr);

  return NULL;
}

#endif
