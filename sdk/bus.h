#ifndef _BUS_h
#define _BUS_h 1

#include <sdk/sdk.h>
#include <sdk/buf.h>
#include <sdk/alloc.h>

typedef struct bus_event_t
{
  u32 id, size;
} bus_event_t;

typedef u32 (*bus_handler_fn) (void *user, bus_event_t *event);

typedef struct bus_t bus_t;

bus_t *bus_create(void *user, allocator_t *allocator);
void   bus_delete(bus_t *bus);

void bus_handler(bus_t *bus, bus_handler_fn fn);

void bus_call(bus_t *bus, bus_event_t *event); // Immediate
void bus_push(bus_t *bus, bus_event_t *event);
void bus_poll(bus_t *bus); // Poll pushed events

#endif
