#ifndef _BUF_h
#define _BUF_h 1

#include <sdk/sdk.h>

typedef struct buffer_t
{
  char *data, *top;
  size_t size, stride;
} buffer_t;

buffer_t *buf_create(size_t reserve, size_t stride);
void buf_delete(buffer_t *buffer);

int buf_push(buffer_t *buffer, const void *src);
int buf_pop(buffer_t *buffer, void *dst);

int buf_full(buffer_t *buffer);
int buf_empty(buffer_t *buffer);

#endif