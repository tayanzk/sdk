#ifndef _HASH_h
#define _HASH_h 1

#include <sdk/sdk.h>

u64 hash_pointer(char *ptr, u64 size)
{
  assert(size == 8);

  return (u64) ptr;
}

u64 hash_fnv64a(char *ptr, u64 size)
{
  u64 hash = 0xcbf29ce484222325;

  for (u32 i = 0; i < size; i++)
    hash = (hash ^ ptr[i]) * 0x100000001b3;

  return hash;
}

#endif
