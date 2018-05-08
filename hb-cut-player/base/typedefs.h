/*
 *  typedefs.h
 *  int type defines
 */

#ifndef __HB_TYPEDEFS_H__
#define __HB_TYPEDEFS_H__

#include "platform.h"

#if defined HB_PLATFORM_LINUX

#include <stdint.h>
#include <stdbool.h>

typedef int8_t hb_int8;
typedef int16_t hb_int16;
typedef int32_t hb_int24;
typedef int32_t hb_int32;
typedef int64_t hb_int64;
typedef uint8_t hb_uint8;
typedef uint16_t hb_uint16;
typedef uint32_t hb_uint24;
typedef uint32_t hb_uint32;
typedef uint64_t hb_uint64;

#elif defined HB_PLATFORM_WIN32

#include <Windows.h>

typedef INT8 hb_int8;
typedef INT16 hb_int16;
typedef INT32 hb_int24;
typedef INT32 hb_int32;
typedef INT64 hb_int64;
typedef UINT8 hb_uint8;
typedef UINT16 hb_uint16;
typedef UINT32 hb_uint24;
typedef UINT32 hb_uint32;
typedef UINT64 hb_uint64;

#ifndef __cplusplus
typedef unsigned short bool;
#define true 1
#define false 0
#endif

#else
#error "non type defines"
#endif

#if !defined(UINT64_MAX)
#define UINT64_MAX ((hb_uint64)(-1))
#endif

#if !defined(INT64_MAX)
#ifdef WIN32
#define INT64_MIN    0x8000000000000000i64
#define INT64_MAX    0x7FFFFFFFFFFFFFFFi64
#else
#define INT64_MIN    0x8000000000000000LL
#define INT64_MAX    0x7FFFFFFFFFFFFFFFLL
#endif
#endif

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

#endif // __HB_TYPEDEFS_H__
