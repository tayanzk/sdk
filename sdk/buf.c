#include <sdk/buf.h>
#include <sdk/sdk.h>

static const uint _Grow = 8; // elements

static inline bool grow(buf_t *buffer, uint count)
{
  uint new = buffer->size + buffer->stride * count;
  uint len = buffer->top - buffer->data;

  void *ptr = reallocate(buffer->allocator, buffer->data, buffer->size, new);
  assert(ptr);

  if (ptr)
  {
    buffer->top  = ptr + len;
    buffer->data = ptr;
    buffer->size = new;

    return true;
  }

  return false;
}

buf_t *buf_create(uint reserve, uint stride, allocator_t *allocator)
{
  buf_t *buffer = allocate(allocator, sizeof(buf_t));
  assert(buffer);

  if (!buffer)
    return NULL;

  buffer->allocator = allocator;
  buffer->stride    = stride;
  buffer->size      = stride * max(_Grow, reserve);
  
  buffer->data = allocate(allocator, buffer->size);
  buffer->top  = buffer->data;
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

static inline uint pushs(buf_t *buffer, const void *src, uint size)
{
  char *bend = buffer->data + buffer->size;
  char *pend = buffer->top + size;

  if (pend >= bend)
  {
    uint count = max((pend - bend + size) / buffer->stride, _Grow);

    if (!grow(buffer, count))
      return 0;
  }

  memcpy(buffer->top, src, size);
  buffer->top += size;

  return buffer->top - buffer->data;
}

uint buf_pushc(buf_t *buffer, const void *src, uint count)
{
  assert(buffer && buffer->data && src);

  return pushs(buffer, src, buffer->stride * count);
}

uint buf_push(buf_t *buffer, const void *src)
{
  assert(buffer && buffer->data && src);

  return pushs(buffer, src, buffer->stride);
}

static inline uint pops(buf_t *buffer, void *dst, uint size)
{
  if (buffer->top == buffer->data)
    return 0;

  if (buffer->top - size < buffer->data)
    return 0;

  buffer->top -= size;
  memcpy(dst, buffer->top, size);

  return buffer->top - buffer->data;
}

uint buf_popc(buf_t *buffer, void *dst, uint count)
{
  assert(buffer && buffer->data && dst);

  return pops(buffer, dst, buffer->stride * count);
}

uint buf_pop(buf_t *buffer, void *dst)
{
  assert(buffer && buffer->data && dst);

  return pops(buffer, dst, buffer->stride);
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
  assert(buffer && buffer->data);
  
  return (buffer->data + buffer->size) <= buffer->top;
}

uint buf_empty(buf_t *buffer)
{
  assert(buffer && buffer->data);
  
  return buffer->top == buffer->data;
}

uint buf_length(buf_t *buffer)
{
  assert(buffer && buffer->data);

  return (buffer->top - buffer->data) / buffer->stride;
}
