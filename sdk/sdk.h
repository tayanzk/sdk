#ifndef _SDK_h
#define _SDK_h 1

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define min(a, b) (((a) > (b)) ? ((b)) : ((a)))
#define max(a, b) (((a) > (b)) ? ((a)) : ((b)))

typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef unsigned long long ullong;

typedef float f32;
typedef double f64;

typedef uint8_t u8;
typedef int8_t s8;

typedef uint16_t u16;
typedef int16_t s16;

typedef uint32_t u32;
typedef int32_t s32;

typedef uint64_t u64;
typedef int64_t s64;

typedef uintptr_t p64;

#endif
