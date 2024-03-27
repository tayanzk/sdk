#include <sdk/buf.h>
#include <sdk/sdk.h>

static const uint _Grow = 8; // elements

static inline bool grow(buffer_t *buffer)
{
  size_t new = buffer->size + buffer->stride * _Grow;

  void *ptr = realloc(buffer->data, new);
  assert(ptr);

  if (ptr)
  {
    buffer->data = ptr;
    buffer->size = new;

    return true;
  }

  return false;
}

buffer_t *buf_create(size_t reserve, size_t stride)
{
  buffer_t *buf = malloc(sizeof(buffer_t));
  assert(buf);

  if (!buf)
    return NULL;

  buf->top    = 0;
  buf->stride = stride;
  buf->size   = stride * min(8, reserve);
  
  buf->data = malloc(buf->size);
  assert(buf->data);

  if (!buf->data)
  {
    free(buf);
    return NULL;
  }

  return buf;
}

void buf_delete(buffer_t *buffer)
{
  assert(buffer && buffer->data);

  free(buffer->data);
  free(buffer);
}

int buf_push(buffer_t *buffer, const void *src)
{
  assert(buffer && buffer->data);

  if (buf_full(buffer))
    if (!grow(buffer))
      return -1;

  memcpy(buffer->top, src, buffer->stride);
  buffer->top += buffer->stride;

  return buffer->top - buffer->data;
}

int buf_pop(buffer_t *buffer, void *dst)
{
  assert(buffer && buffer->data);

  if (buf_empty(buffer))
    return -1;

  buffer->top -= buffer->stride;
  memcpy(dst, buffer->top, buffer->stride);

  return buffer->top - buffer->data;
}

int buf_full(buffer_t *buffer)
{
  return buffer->data + buffer->size == buffer->top;
}

int buf_empty(buffer_t *buffer)
{
  return buffer->top == buffer->data;
}