#ifndef _STRING_h
#define _STRING_h 1

#include <sdk/sdk.h>

typedef struct string_t
{
  cstr string;
  uint length;
} string_t;

#define string_view(s) (string_t) { .string = s, .length = strlen(s) }

#endif
