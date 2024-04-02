#ifndef _BUF_h
#define _BUF_h 1

#include <sdk/sdk.h>
#include <sdk/alloc.h>

typedef struct buf_t
{
  char *data, *top;
  size_t size, stride;

  allocator_t *allocator;
} buf_t;

buf_t *buf_create(size_t reserve, size_t stride, allocator_t *allocator);
void buf_delete(buf_t *buffer);

int buf_push(buf_t *buffer, const void *src);
int buf_pop(buf_t *buffer, void *dst);
void *buf_next(buf_t *buffer);

int buf_full(buf_t *buffer);
int buf_empty(buf_t *buffer);
int bus_length(buf_t *buffer);

#endif
