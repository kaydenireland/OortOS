#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

// Unsigned Integer Types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;


// Signed Integer Types
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


// Floating Point Types
typedef float f32;
typedef double f64;


// Boolean Types
typedef u8 b8;

#define true 1
#define false 0

#define null 0


// Maximums
#define MAX_U8 255
#define MAX_U16 65535
#define MAX_U32 4294967295
#define MAX_U64 18446744073709551615

#define MAX_I8 127
#define MAX_I16 32767
#define MAX_I32 2147483647
#define MAX_I64 9223372036854775807

// Minimums
#define MIN_I8 -127
#define MIN_I16 -32768
#define MIN_I32 -2147483648
#define MIN_I64 -9223372036854775808

#endif