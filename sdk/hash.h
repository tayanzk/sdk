#ifndef _HASH_h
#define _HASH_h 1

#include <sdk/sdk.h>

u64 hash_pointer(char *ptr, u64 size);
u64 hash_fnv64a(char *ptr, u64 size);

#endif
