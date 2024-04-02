#include <sdk/buf.h>
#include <sdk/sdk.h>

static const uint _Grow = 8; // elements

static inline bool grow(buf_t *buffer)
{
  uint new = buffer->size + buffer->stride * _Grow;

  void *ptr = reallocate(buffer->allocator, buffer->data, buffer->size, new);
  assert(ptr);

  if (ptr)
  {
    buffer->data = ptr;
    buffer->size = new;

    return true;
  }

  return false;
}

buf_t *buf_create(uint reserve, uint stride, allocator_t *allocator)
{
  buf_t *buffer = allocate(allocator, sizeof(buffer_t));
  assert(buffer);

  if (!buffer)
    return NULL;

  buffer->allocator = allocator;

  buffer->top    = 0;
  buffer->stride = stride;
  buffer->size   = stride * min(8, reserve);
  
  buffer->data = allocate(allocator, buffer->size);
  assert(buffer->data);

  if (!buffer->data)
  {
    deallocate(allocator, buffer);
    return NULL;
  }

  return buffer;
}

void buf_delete(buf_t *buffer)
{
  assert(buffer && buffer->data);

  deallocate(buffer->allocator, buffer->data);
  deallocate(buffer->allocator, buffer);
}

uint buf_push(buf_t *buffer, const void *src)
{
  assert(buffer && buffer->data && src);

  if (buf_full(buffer))
    if (!grow(buffer))
      return -1;

  memcpy(buffer->top, src, buffer->stride);
  buffer->top += buffer->stride;

  return buffer->top - buffer->data;
}

uint buf_pop(buf_t *buffer, void *dst)
{
  assert(buffer && buffer->data && dst);

  if (buf_empty(buffer))
    return -1;

  buffer->top -= buffer->stride;
  memcpy(dst, buffer->top, buffer->stride);

  return buffer->top - buffer->data;
}

void *buf_next(buf_t *buffer)
{
  assert(buffer && buffer->data);

  void *ptr = buffer->top;
  buffer->top += buffer->stride;

  return ptr;
}

uint buf_full(buf_t *buffer)
{
  return buffer->data + buffer->size == buffer->top;
}

uint buf_empty(buf_t *buffer)
{
  return buffer->top == buffer->data;
}

uint buf_length(buf_t *buffer)
{
  return buffer->size / buffer->stride;
}
