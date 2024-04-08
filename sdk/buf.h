#ifndef _BUF_h
#define _BUF_h 1

#include <sdk/sdk.h>
#include <sdk/alloc.h>

typedef struct buf_t
{
  char *data, *top;
  uint size, stride;

  allocator_t *allocator;
} buf_t;

buf_t *buf_create(uint reserve, uint stride, allocator_t *allocator);
void buf_delete(buf_t *buffer);

uint buf_pushc(buf_t *buffer, const void *src, uint count);
uint buf_push(buf_t *buffer, const void *src);
uint buf_popc(buf_t *buffer, void *dst, uint count);
uint buf_pop(buf_t *buffer, void *dst);
void *buf_next(buf_t *buffer);

uint buf_full(buf_t *buffer);
uint buf_empty(buf_t *buffer);
uint buf_length(buf_t *buffer);

#endif
