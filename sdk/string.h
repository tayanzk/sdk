#ifndef _STRING_h
#define _STRING_h 1

#include <sdk/sdk.h>

typedef struct string_t
{
  cstr string;
  uint length;
} string_t;

static inline string_t string(cstr string)
{
  return (string_t) { .string = string, .length = strlen(string) };
}

#endif
