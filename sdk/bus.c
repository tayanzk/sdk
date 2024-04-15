#include <sdk/bus.h>

struct bus_t
{
  void *user;

  buf_t *handlers;
  bus_t *events;
};

bus_t *bus_create(void *user, allocator_t *allocator)
{
  bus_t *bus = allocate(allocator, sizeof(bus_t));
  assert(bus != NULL);

  if (!bus)
    return NULL;

  bus->handlers = buf_create(0, sizeof(bus_handler_fn), allocator);
  bus->events   = buf_create(0, 1, allocator);
  bus->user     = user;

  return bus;
}

void bus_delete(bus_t *bus)
{
  assert(bus && bus->handlers && bus->events);

  buf_delete(bus->handlers);
  buf_delete(bus->events);

  deallocate(bus);
}

void bus_handler(bus_t *bus, bus_handler_fn fn)
{
  assert(bus && bus->handlers && bus->events);

  buf_push(bus->handlers, fn);
}

void bus_call(bus_t *bus, bus_event_t *event)
{
  assert(bus && bus->handlers && bus->events);
  assert(event->size >= sizeof(bus_event_t) && event->size <= 2048);

  uint length = buf_length(bus->handlers);
  bus_handler_fn *handlers = bus->handlers->data;

  for (uint i = 0; i < length; i++)
  {
    if (handlers[i](bus->user, event))
      break;
  }
}

void bus_push(bus_t *bus, bus_event_t *event)
{
  assert(bus && bus->handlers && bus->events);
  assert(event->size >= sizeof(bus_event_t) && event->size <= 2048);

  buf_pushc(bus->events, event, event->size);
}

void bus_poll(bus_t *bus)
{
  assert(bus && bus->handlers && bus->events);
  bus_event_t *event = bus->handlers->data;

  while (event < buf->events->top)
  {
    assert(event->size >= sizeof(bus_event_t) && event->size <= 2048);
  
    bus_call(bus, event);
    event += event->size;
  }
}
