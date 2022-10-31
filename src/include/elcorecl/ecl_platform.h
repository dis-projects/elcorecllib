// Copyright 2018-2019 RnD Center "ELVEES", JSC
/**********************************************************************************
 * Copyright (c) 2008-2018 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * MODIFICATIONS TO THIS FILE MAY MEAN IT NO LONGER ACCURATELY REFLECTS
 * KHRONOS STANDARDS. THE UNMODIFIED, NORMATIVE VERSIONS OF KHRONOS
 * SPECIFICATIONS AND HEADER INFORMATION ARE LOCATED AT
 *    https://www.khronos.org/registry/
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 **********************************************************************************/

#ifndef __ELCORE_ECL_PLATFORM_H
#define __ELCORE_ECL_PLATFORM_H

#include <elcorecl/ecl_version.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
    #define ECL_API_ENTRY
    #define ECL_API_CALL     __stdcall
    #define ECL_CALLBACK     __stdcall
#else
    #define ECL_API_ENTRY
    #define ECL_API_CALL
    #define ECL_CALLBACK
#endif

/*
 * Deprecation flags refer to the last version of the header in which the
 * feature was not deprecated.
 *
 * E.g. VERSION_1_1_DEPRECATED means the feature is present in 1.1 without
 * deprecation but is deprecated in versions later than 1.1.
 */

#define ECL_EXTENSION_WEAK_LINK
#define ECL_API_SUFFIX__VERSION_1_0
#define ECL_EXT_SUFFIX__VERSION_1_0
#define ECL_API_SUFFIX__VERSION_1_1
#define ECL_EXT_SUFFIX__VERSION_1_1
#define ECL_API_SUFFIX__VERSION_1_2
#define ECL_EXT_SUFFIX__VERSION_1_2
#define ECL_API_SUFFIX__VERSION_2_0
#define ECL_EXT_SUFFIX__VERSION_2_0
#define ECL_API_SUFFIX__VERSION_2_1
#define ECL_EXT_SUFFIX__VERSION_2_1
#define ECL_API_SUFFIX__VERSION_2_2
#define ECL_EXT_SUFFIX__VERSION_2_2

#ifdef __GNUC__
    #ifdef ECL_USE_DEPRECATED_OPENCL_1_0_APIS
        #define ECL_EXT_SUFFIX__VERSION_1_0_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_1_0_DEPRECATED
    #else
        #define ECL_EXT_SUFFIX__VERSION_1_0_DEPRECATED __attribute__((deprecated))
        #define ECL_EXT_PREFIX__VERSION_1_0_DEPRECATED
    #endif

    #ifdef ECL_USE_DEPRECATED_OPENCL_1_1_APIS
        #define ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED
    #else
        #define ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED __attribute__((deprecated))
        #define ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED
    #endif

    #ifdef ECL_USE_DEPRECATED_OPENCL_1_2_APIS
        #define ECL_EXT_SUFFIX__VERSION_1_2_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_1_2_DEPRECATED
    #else
        #define ECL_EXT_SUFFIX__VERSION_1_2_DEPRECATED __attribute__((deprecated))
        #define ECL_EXT_PREFIX__VERSION_1_2_DEPRECATED
     #endif

    #ifdef ECL_USE_DEPRECATED_OPENCL_2_0_APIS
        #define ECL_EXT_SUFFIX__VERSION_2_0_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_2_0_DEPRECATED
    #else
        #define ECL_EXT_SUFFIX__VERSION_2_0_DEPRECATED __attribute__((deprecated))
        #define ECL_EXT_PREFIX__VERSION_2_0_DEPRECATED
    #endif

    #ifdef ECL_USE_DEPRECATED_OPENCL_2_1_APIS
        #define ECL_EXT_SUFFIX__VERSION_2_1_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_2_1_DEPRECATED
    #else
        #define ECL_EXT_SUFFIX__VERSION_2_1_DEPRECATED __attribute__((deprecated))
        #define ECL_EXT_PREFIX__VERSION_2_1_DEPRECATED
    #endif
#elif defined(_WIN32)
    #ifdef ECL_USE_DEPRECATED_OPENCL_1_0_APIS
        #define ECL_EXT_SUFFIX__VERSION_1_0_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_1_0_DEPRECATED
    #else
        #define ECL_EXT_SUFFIX__VERSION_1_0_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_1_0_DEPRECATED __declspec(deprecated)
    #endif

    #ifdef ECL_USE_DEPRECATED_OPENCL_1_1_APIS
        #define ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED
    #else
        #define ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED __declspec(deprecated)
    #endif

    #ifdef ECL_USE_DEPRECATED_OPENCL_1_2_APIS
        #define ECL_EXT_SUFFIX__VERSION_1_2_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_1_2_DEPRECATED
    #else
        #define ECL_EXT_SUFFIX__VERSION_1_2_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_1_2_DEPRECATED __declspec(deprecated)
    #endif

    #ifdef ECL_USE_DEPRECATED_OPENCL_2_0_APIS
        #define ECL_EXT_SUFFIX__VERSION_2_0_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_2_0_DEPRECATED
    #else
        #define ECL_EXT_SUFFIX__VERSION_2_0_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_2_0_DEPRECATED __declspec(deprecated)
    #endif

    #ifdef ECL_USE_DEPRECATED_OPENCL_2_1_APIS
        #define ECL_EXT_SUFFIX__VERSION_2_1_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_2_1_DEPRECATED
    #else
        #define ECL_EXT_SUFFIX__VERSION_2_1_DEPRECATED
        #define ECL_EXT_PREFIX__VERSION_2_1_DEPRECATED __declspec(deprecated)
    #endif
#else
    #define ECL_EXT_SUFFIX__VERSION_1_0_DEPRECATED
    #define ECL_EXT_PREFIX__VERSION_1_0_DEPRECATED

    #define ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED
    #define ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED

    #define ECL_EXT_SUFFIX__VERSION_1_2_DEPRECATED
    #define ECL_EXT_PREFIX__VERSION_1_2_DEPRECATED

    #define ECL_EXT_SUFFIX__VERSION_2_0_DEPRECATED
    #define ECL_EXT_PREFIX__VERSION_2_0_DEPRECATED

    #define ECL_EXT_SUFFIX__VERSION_2_1_DEPRECATED
    #define ECL_EXT_PREFIX__VERSION_2_1_DEPRECATED
#endif

#if (defined (_WIN32) && defined(_MSC_VER))

/* scalar types  */
typedef signed   __int8         ecl_char;
typedef unsigned __int8         ecl_uchar;
typedef signed   __int16        ecl_short;
typedef unsigned __int16        ecl_ushort;
typedef signed   __int32        ecl_int;
typedef unsigned __int32        ecl_uint;
typedef signed   __int64        ecl_long;
typedef unsigned __int64        ecl_ulong;

typedef unsigned __int16        ecl_half;
typedef float                   ecl_float;
typedef double                  ecl_double;

/* Macro names and corresponding values defined by OpenCL */
#define ECL_CHAR_BIT         8
#define ECL_SCHAR_MAX        127
#define ECL_SCHAR_MIN        (-127-1)
#define ECL_CHAR_MAX         ECL_SCHAR_MAX
#define ECL_CHAR_MIN         ECL_SCHAR_MIN
#define ECL_UCHAR_MAX        255
#define ECL_SHRT_MAX         32767
#define ECL_SHRT_MIN         (-32767-1)
#define ECL_USHRT_MAX        65535
#define ECL_INT_MAX          2147483647
#define ECL_INT_MIN          (-2147483647-1)
#define ECL_UINT_MAX         0xffffffffU
#define ECL_LONG_MAX         ((ecl_long) 0x7FFFFFFFFFFFFFFFLL)
#define ECL_LONG_MIN         ((ecl_long) -0x7FFFFFFFFFFFFFFFLL - 1LL)
#define ECL_ULONG_MAX        ((ecl_ulong) 0xFFFFFFFFFFFFFFFFULL)

#define ECL_FLT_DIG          6
#define ECL_FLT_MANT_DIG     24
#define ECL_FLT_MAX_10_EXP   +38
#define ECL_FLT_MAX_EXP      +128
#define ECL_FLT_MIN_10_EXP   -37
#define ECL_FLT_MIN_EXP      -125
#define ECL_FLT_RADIX        2
#define ECL_FLT_MAX          340282346638528859811704183484516925440.0f
#define ECL_FLT_MIN          1.175494350822287507969e-38f
#define ECL_FLT_EPSILON      1.1920928955078125e-7f

#define ECL_HALF_DIG          3
#define ECL_HALF_MANT_DIG     11
#define ECL_HALF_MAX_10_EXP   +4
#define ECL_HALF_MAX_EXP      +16
#define ECL_HALF_MIN_10_EXP   -4
#define ECL_HALF_MIN_EXP      -13
#define ECL_HALF_RADIX        2
#define ECL_HALF_MAX          65504.0f
#define ECL_HALF_MIN          6.103515625e-05f
#define ECL_HALF_EPSILON      9.765625e-04f

#define ECL_DBL_DIG          15
#define ECL_DBL_MANT_DIG     53
#define ECL_DBL_MAX_10_EXP   +308
#define ECL_DBL_MAX_EXP      +1024
#define ECL_DBL_MIN_10_EXP   -307
#define ECL_DBL_MIN_EXP      -1021
#define ECL_DBL_RADIX        2
#define ECL_DBL_MAX          1.7976931348623158e+308
#define ECL_DBL_MIN          2.225073858507201383090e-308
#define ECL_DBL_EPSILON      2.220446049250313080847e-16

#define ECL_M_E              2.7182818284590452354
#define ECL_M_LOG2E          1.4426950408889634074
#define ECL_M_LOG10E         0.43429448190325182765
#define ECL_M_LN2            0.69314718055994530942
#define ECL_M_LN10           2.30258509299404568402
#define ECL_M_PI             3.14159265358979323846
#define ECL_M_PI_2           1.57079632679489661923
#define ECL_M_PI_4           0.78539816339744830962
#define ECL_M_1_PI           0.31830988618379067154
#define ECL_M_2_PI           0.63661977236758134308
#define ECL_M_2_SQRTPI       1.12837916709551257390
#define ECL_M_SQRT2          1.41421356237309504880
#define ECL_M_SQRT1_2        0.70710678118654752440

#define ECL_M_E_F            2.718281828f
#define ECL_M_LOG2E_F        1.442695041f
#define ECL_M_LOG10E_F       0.434294482f
#define ECL_M_LN2_F          0.693147181f
#define ECL_M_LN10_F         2.302585093f
#define ECL_M_PI_F           3.141592654f
#define ECL_M_PI_2_F         1.570796327f
#define ECL_M_PI_4_F         0.785398163f
#define ECL_M_1_PI_F         0.318309886f
#define ECL_M_2_PI_F         0.636619772f
#define ECL_M_2_SQRTPI_F     1.128379167f
#define ECL_M_SQRT2_F        1.414213562f
#define ECL_M_SQRT1_2_F      0.707106781f

#define ECL_NAN              (ECL_INFINITY - ECL_INFINITY)
#define ECL_HUGE_VALF        ((ecl_float) 1e50)
#define ECL_HUGE_VAL         ((ecl_double) 1e500)
#define ECL_MAXFLOAT         ECL_FLT_MAX
#define ECL_INFINITY         ECL_HUGE_VALF

#else

#include <stdint.h>

/* scalar types  */
typedef int8_t          ecl_char;
typedef uint8_t         ecl_uchar;
typedef int16_t         ecl_short    __attribute__((aligned(2)));
typedef uint16_t        ecl_ushort   __attribute__((aligned(2)));
typedef int32_t         ecl_int      __attribute__((aligned(4)));
typedef uint32_t        ecl_uint     __attribute__((aligned(4)));
typedef int64_t         ecl_long     __attribute__((aligned(8)));
typedef uint64_t        ecl_ulong    __attribute__((aligned(8)));

typedef uint16_t        ecl_half     __attribute__((aligned(2)));
typedef float           ecl_float    __attribute__((aligned(4)));
typedef double          ecl_double   __attribute__((aligned(8)));

/* Macro names and corresponding values defined by OpenCL */
#define ECL_CHAR_BIT         8
#define ECL_SCHAR_MAX        127
#define ECL_SCHAR_MIN        (-127-1)
#define ECL_CHAR_MAX         ECL_SCHAR_MAX
#define ECL_CHAR_MIN         ECL_SCHAR_MIN
#define ECL_UCHAR_MAX        255
#define ECL_SHRT_MAX         32767
#define ECL_SHRT_MIN         (-32767-1)
#define ECL_USHRT_MAX        65535
#define ECL_INT_MAX          2147483647
#define ECL_INT_MIN          (-2147483647-1)
#define ECL_UINT_MAX         0xffffffffU
#define ECL_LONG_MAX         ((ecl_long) 0x7FFFFFFFFFFFFFFFLL)
#define ECL_LONG_MIN         ((ecl_long) -0x7FFFFFFFFFFFFFFFLL - 1LL)
#define ECL_ULONG_MAX        ((ecl_ulong) 0xFFFFFFFFFFFFFFFFULL)

#define ECL_FLT_DIG          6
#define ECL_FLT_MANT_DIG     24
#define ECL_FLT_MAX_10_EXP   +38
#define ECL_FLT_MAX_EXP      +128
#define ECL_FLT_MIN_10_EXP   -37
#define ECL_FLT_MIN_EXP      -125
#define ECL_FLT_RADIX        2
#define ECL_FLT_MAX          340282346638528859811704183484516925440.0f
#define ECL_FLT_MIN          1.175494350822287507969e-38f
#define ECL_FLT_EPSILON      1.1920928955078125e-7f

#define ECL_HALF_DIG          3
#define ECL_HALF_MANT_DIG     11
#define ECL_HALF_MAX_10_EXP   +4
#define ECL_HALF_MAX_EXP      +16
#define ECL_HALF_MIN_10_EXP   -4
#define ECL_HALF_MIN_EXP      -13
#define ECL_HALF_RADIX        2
#define ECL_HALF_MAX          65504.0f
#define ECL_HALF_MIN          6.103515625e-05f
#define ECL_HALF_EPSILON      9.765625e-04f

#define ECL_DBL_DIG          15
#define ECL_DBL_MANT_DIG     53
#define ECL_DBL_MAX_10_EXP   +308
#define ECL_DBL_MAX_EXP      +1024
#define ECL_DBL_MIN_10_EXP   -307
#define ECL_DBL_MIN_EXP      -1021
#define ECL_DBL_RADIX        2
#define ECL_DBL_MAX          179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0
#define ECL_DBL_MIN          2.225073858507201383090e-308
#define ECL_DBL_EPSILON      2.220446049250313080847e-16

#define ECL_M_E              2.7182818284590452354
#define ECL_M_LOG2E          1.4426950408889634074
#define ECL_M_LOG10E         0.43429448190325182765
#define ECL_M_LN2            0.69314718055994530942
#define ECL_M_LN10           2.30258509299404568402
#define ECL_M_PI             3.14159265358979323846
#define ECL_M_PI_2           1.57079632679489661923
#define ECL_M_PI_4           0.78539816339744830962
#define ECL_M_1_PI           0.31830988618379067154
#define ECL_M_2_PI           0.63661977236758134308
#define ECL_M_2_SQRTPI       1.12837916709551257390
#define ECL_M_SQRT2          1.41421356237309504880
#define ECL_M_SQRT1_2        0.70710678118654752440

#define ECL_M_E_F            2.718281828f
#define ECL_M_LOG2E_F        1.442695041f
#define ECL_M_LOG10E_F       0.434294482f
#define ECL_M_LN2_F          0.693147181f
#define ECL_M_LN10_F         2.302585093f
#define ECL_M_PI_F           3.141592654f
#define ECL_M_PI_2_F         1.570796327f
#define ECL_M_PI_4_F         0.785398163f
#define ECL_M_1_PI_F         0.318309886f
#define ECL_M_2_PI_F         0.636619772f
#define ECL_M_2_SQRTPI_F     1.128379167f
#define ECL_M_SQRT2_F        1.414213562f
#define ECL_M_SQRT1_2_F      0.707106781f

#if defined( __GNUC__ )
   #define ECL_HUGE_VALF     __builtin_huge_valf()
   #define ECL_HUGE_VAL      __builtin_huge_val()
   #define ECL_NAN           __builtin_nanf( "" )
#else
   #define ECL_HUGE_VALF     ((ecl_float) 1e50)
   #define ECL_HUGE_VAL      ((ecl_double) 1e500)
   float nanf( const char * );
   #define ECL_NAN           nanf( "" )
#endif
#define ECL_MAXFLOAT         ECL_FLT_MAX
#define ECL_INFINITY         ECL_HUGE_VALF

#endif

#include <stddef.h>

/* Mirror types to GL types. Mirror types allow us to avoid deciding which 87s to load based on whether we are using GL or GLES here. */
typedef unsigned int ecl_GLuint;
typedef int          ecl_GLint;
typedef unsigned int ecl_GLenum;

/*
 * Vector types
 *
 *  Note:   OpenCL requires that all types be naturally aligned.
 *          This means that vector types must be naturally aligned.
 *          For example, a vector of four floats must be aligned to
 *          a 16 byte boundary (calculated as 4 * the natural 4-byte
 *          alignment of the float).  The alignment qualifiers here
 *          will only function properly if your compiler supports them
 *          and if you don't actively work to defeat them.  For example,
 *          in order for a ecl_float4 to be 16 byte aligned in a struct,
 *          the start of the struct must itself be 16-byte aligned.
 *
 *          Maintaining proper alignment is the user's responsibility.
 */

/* Define basic vector types */
#if defined( __VEC__ )
   #include <altivec.h>   /* may be omitted depending on compiler. AltiVec spec provides no way to detect whether the header is required. */
   typedef vector unsigned char     __ecl_uchar16;
   typedef vector signed char       __ecl_char16;
   typedef vector unsigned short    __ecl_ushort8;
   typedef vector signed short      __ecl_short8;
   typedef vector unsigned int      __ecl_uint4;
   typedef vector signed int        __ecl_int4;
   typedef vector float             __ecl_float4;
   #define  __ECL_UCHAR16__  1
   #define  __ECL_CHAR16__   1
   #define  __ECL_USHORT8__  1
   #define  __ECL_SHORT8__   1
   #define  __ECL_UINT4__    1
   #define  __ECL_INT4__     1
   #define  __ECL_FLOAT4__   1
#endif

#if defined( __SSE__ )
    #if defined( __MINGW64__ )
        #include <intrin.h>
    #else
        #include <xmmintrin.h>
    #endif
    #if defined( __GNUC__ )
        typedef float __ecl_float4   __attribute__((vector_size(16)));
    #else
        typedef __m128 __ecl_float4;
    #endif
    #define __ECL_FLOAT4__   1
#endif

#if defined( __SSE2__ )
    #if defined( __MINGW64__ )
        #include <intrin.h>
    #else
        #include <emmintrin.h>
    #endif
    #if defined( __GNUC__ )
        typedef ecl_uchar    __ecl_uchar16    __attribute__((vector_size(16)));
        typedef ecl_char     __ecl_char16     __attribute__((vector_size(16)));
        typedef ecl_ushort   __ecl_ushort8    __attribute__((vector_size(16)));
        typedef ecl_short    __ecl_short8     __attribute__((vector_size(16)));
        typedef ecl_uint     __ecl_uint4      __attribute__((vector_size(16)));
        typedef ecl_int      __ecl_int4       __attribute__((vector_size(16)));
        typedef ecl_ulong    __ecl_ulong2     __attribute__((vector_size(16)));
        typedef ecl_long     __ecl_long2      __attribute__((vector_size(16)));
        typedef ecl_double   __ecl_double2    __attribute__((vector_size(16)));
    #else
        typedef __m128i __ecl_uchar16;
        typedef __m128i __ecl_char16;
        typedef __m128i __ecl_ushort8;
        typedef __m128i __ecl_short8;
        typedef __m128i __ecl_uint4;
        typedef __m128i __ecl_int4;
        typedef __m128i __ecl_ulong2;
        typedef __m128i __ecl_long2;
        typedef __m128d __ecl_double2;
    #endif
    #define __ECL_UCHAR16__  1
    #define __ECL_CHAR16__   1
    #define __ECL_USHORT8__  1
    #define __ECL_SHORT8__   1
    #define __ECL_INT4__     1
    #define __ECL_UINT4__    1
    #define __ECL_ULONG2__   1
    #define __ECL_LONG2__    1
    #define __ECL_DOUBLE2__  1
#endif

#if defined( __MMX__ )
    #include <mmintrin.h>
    #if defined( __GNUC__ )
        typedef ecl_uchar    __ecl_uchar8     __attribute__((vector_size(8)));
        typedef ecl_char     __ecl_char8      __attribute__((vector_size(8)));
        typedef ecl_ushort   __ecl_ushort4    __attribute__((vector_size(8)));
        typedef ecl_short    __ecl_short4     __attribute__((vector_size(8)));
        typedef ecl_uint     __ecl_uint2      __attribute__((vector_size(8)));
        typedef ecl_int      __ecl_int2       __attribute__((vector_size(8)));
        typedef ecl_ulong    __ecl_ulong1     __attribute__((vector_size(8)));
        typedef ecl_long     __ecl_long1      __attribute__((vector_size(8)));
        typedef ecl_float    __ecl_float2     __attribute__((vector_size(8)));
    #else
        typedef __m64       __ecl_uchar8;
        typedef __m64       __ecl_char8;
        typedef __m64       __ecl_ushort4;
        typedef __m64       __ecl_short4;
        typedef __m64       __ecl_uint2;
        typedef __m64       __ecl_int2;
        typedef __m64       __ecl_ulong1;
        typedef __m64       __ecl_long1;
        typedef __m64       __ecl_float2;
    #endif
    #define __ECL_UCHAR8__   1
    #define __ECL_CHAR8__    1
    #define __ECL_USHORT4__  1
    #define __ECL_SHORT4__   1
    #define __ECL_INT2__     1
    #define __ECL_UINT2__    1
    #define __ECL_ULONG1__   1
    #define __ECL_LONG1__    1
    #define __ECL_FLOAT2__   1
#endif

#if defined( __AVX__ )
    #if defined( __MINGW64__ )
        #include <intrin.h>
    #else
        #include <immintrin.h>
    #endif
    #if defined( __GNUC__ )
        typedef ecl_float    __ecl_float8     __attribute__((vector_size(32)));
        typedef ecl_double   __ecl_double4    __attribute__((vector_size(32)));
    #else
        typedef __m256      __ecl_float8;
        typedef __m256d     __ecl_double4;
    #endif
    #define __ECL_FLOAT8__   1
    #define __ECL_DOUBLE4__  1
#endif

/* Define capabilities for anonymous struct members. */
#if !defined(__cplusplus) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define  __ECL_HAS_ANON_STRUCT__ 1
#define  __ECL_ANON_STRUCT__
#elif defined( __GNUC__) && ! defined( __STRICT_ANSI__ )
#define  __ECL_HAS_ANON_STRUCT__ 1
#define  __ECL_ANON_STRUCT__ __extension__
#elif defined( _WIN32) && defined(_MSC_VER)
    #if _MSC_VER >= 1500
   /* Microsoft Developer Studio 2008 supports anonymous structs, but
    * complains by default. */
    #define  __ECL_HAS_ANON_STRUCT__ 1
    #define  __ECL_ANON_STRUCT__
   /* Disable warning C4201: nonstandard extension used : nameless
    * struct/union */
    #pragma warning( push )
    #pragma warning( disable : 4201 )
    #endif
#else
#define  __ECL_HAS_ANON_STRUCT__ 0
#define  __ECL_ANON_STRUCT__
#endif

/* Define alignment keys */
#if defined( __GNUC__ )
    #define ECL_ALIGNED(_x)          __attribute__ ((aligned(_x)))
#elif defined( _WIN32) && (_MSC_VER)
    /* Alignment keys neutered on windows because MSVC can't swallow function arguments with alignment requirements     */
    /* http://msdn.microsoft.com/en-us/library/373ak2y1%28VS.71%29.aspx                                                 */
    /* #include <crtdefs.h>                                                                                             */
    /* #define ECL_ALIGNED(_x)          _CRT_ALIGN(_x)                                                                   */
    #define ECL_ALIGNED(_x)
#else
   #warning  Need to implement some method to align data here
   #define  ECL_ALIGNED(_x)
#endif

/* Indicate whether .xyzw, .s0123 and .hi.lo are supported */
#if __ECL_HAS_ANON_STRUCT__
    /* .xyzw and .s0123...{f|F} are supported */
    #define ECL_HAS_NAMED_VECTOR_FIELDS 1
    /* .hi and .lo are supported */
    #define ECL_HAS_HI_LO_VECTOR_FIELDS 1
#endif

/* Define ecl_vector types */

/* ---- ecl_charn ---- */
typedef union
{
    ecl_char  ECL_ALIGNED(2) s[2];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_char  x, y; };
   __ECL_ANON_STRUCT__ struct{ ecl_char  s0, s1; };
   __ECL_ANON_STRUCT__ struct{ ecl_char  lo, hi; };
#endif
#if defined( __ECL_CHAR2__)
    __ecl_char2     v2;
#endif
}ecl_char2;

typedef union
{
    ecl_char  ECL_ALIGNED(4) s[4];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_char  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_char  s0, s1, s2, s3; };
   __ECL_ANON_STRUCT__ struct{ ecl_char2 lo, hi; };
#endif
#if defined( __ECL_CHAR2__)
    __ecl_char2     v2[2];
#endif
#if defined( __ECL_CHAR4__)
    __ecl_char4     v4;
#endif
}ecl_char4;

/* ecl_char3 is identical in size, alignment and behavior to ecl_char4. See section 6.1.5. */
typedef  ecl_char4  ecl_char3;

typedef union
{
    ecl_char   ECL_ALIGNED(8) s[8];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_char  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_char  s0, s1, s2, s3, s4, s5, s6, s7; };
   __ECL_ANON_STRUCT__ struct{ ecl_char4 lo, hi; };
#endif
#if defined( __ECL_CHAR2__)
    __ecl_char2     v2[4];
#endif
#if defined( __ECL_CHAR4__)
    __ecl_char4     v4[2];
#endif
#if defined( __ECL_CHAR8__ )
    __ecl_char8     v8;
#endif
}ecl_char8;

typedef union
{
    ecl_char  ECL_ALIGNED(16) s[16];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_char  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __ECL_ANON_STRUCT__ struct{ ecl_char  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __ECL_ANON_STRUCT__ struct{ ecl_char8 lo, hi; };
#endif
#if defined( __ECL_CHAR2__)
    __ecl_char2     v2[8];
#endif
#if defined( __ECL_CHAR4__)
    __ecl_char4     v4[4];
#endif
#if defined( __ECL_CHAR8__ )
    __ecl_char8     v8[2];
#endif
#if defined( __ECL_CHAR16__ )
    __ecl_char16    v16;
#endif
}ecl_char16;


/* ---- ecl_ucharn ---- */
typedef union
{
    ecl_uchar  ECL_ALIGNED(2) s[2];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_uchar  x, y; };
   __ECL_ANON_STRUCT__ struct{ ecl_uchar  s0, s1; };
   __ECL_ANON_STRUCT__ struct{ ecl_uchar  lo, hi; };
#endif
#if defined( __ecl_uchar2__)
    __ecl_uchar2     v2;
#endif
}ecl_uchar2;

typedef union
{
    ecl_uchar  ECL_ALIGNED(4) s[4];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_uchar  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_uchar  s0, s1, s2, s3; };
   __ECL_ANON_STRUCT__ struct{ ecl_uchar2 lo, hi; };
#endif
#if defined( __ECL_UCHAR2__)
    __ecl_uchar2     v2[2];
#endif
#if defined( __ECL_UCHAR4__)
    __ecl_uchar4     v4;
#endif
}ecl_uchar4;

/* ecl_uchar3 is identical in size, alignment and behavior to ecl_uchar4. See section 6.1.5. */
typedef  ecl_uchar4  ecl_uchar3;

typedef union
{
    ecl_uchar   ECL_ALIGNED(8) s[8];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_uchar  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_uchar  s0, s1, s2, s3, s4, s5, s6, s7; };
   __ECL_ANON_STRUCT__ struct{ ecl_uchar4 lo, hi; };
#endif
#if defined( __ECL_UCHAR2__)
    __ecl_uchar2     v2[4];
#endif
#if defined( __ECL_UCHAR4__)
    __ecl_uchar4     v4[2];
#endif
#if defined( __ECL_UCHAR8__ )
    __ecl_uchar8     v8;
#endif
}ecl_uchar8;

typedef union
{
    ecl_uchar  ECL_ALIGNED(16) s[16];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_uchar  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __ECL_ANON_STRUCT__ struct{ ecl_uchar  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __ECL_ANON_STRUCT__ struct{ ecl_uchar8 lo, hi; };
#endif
#if defined( __ECL_UCHAR2__)
    __ecl_uchar2     v2[8];
#endif
#if defined( __ECL_UCHAR4__)
    __ecl_uchar4     v4[4];
#endif
#if defined( __ECL_UCHAR8__ )
    __ecl_uchar8     v8[2];
#endif
#if defined( __ECL_UCHAR16__ )
    __ecl_uchar16    v16;
#endif
}ecl_uchar16;


/* ---- ecl_shortn ---- */
typedef union
{
    ecl_short  ECL_ALIGNED(4) s[2];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_short  x, y; };
   __ECL_ANON_STRUCT__ struct{ ecl_short  s0, s1; };
   __ECL_ANON_STRUCT__ struct{ ecl_short  lo, hi; };
#endif
#if defined( __ECL_SHORT2__)
    __ecl_short2     v2;
#endif
}ecl_short2;

typedef union
{
    ecl_short  ECL_ALIGNED(8) s[4];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_short  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_short  s0, s1, s2, s3; };
   __ECL_ANON_STRUCT__ struct{ ecl_short2 lo, hi; };
#endif
#if defined( __ECL_SHORT2__)
    __ecl_short2     v2[2];
#endif
#if defined( __ECL_SHORT4__)
    __ecl_short4     v4;
#endif
}ecl_short4;

/* ecl_short3 is identical in size, alignment and behavior to ecl_short4. See section 6.1.5. */
typedef  ecl_short4  ecl_short3;

typedef union
{
    ecl_short   ECL_ALIGNED(16) s[8];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_short  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_short  s0, s1, s2, s3, s4, s5, s6, s7; };
   __ECL_ANON_STRUCT__ struct{ ecl_short4 lo, hi; };
#endif
#if defined( __ECL_SHORT2__)
    __ecl_short2     v2[4];
#endif
#if defined( __ECL_SHORT4__)
    __ecl_short4     v4[2];
#endif
#if defined( __ECL_SHORT8__ )
    __ecl_short8     v8;
#endif
}ecl_short8;

typedef union
{
    ecl_short  ECL_ALIGNED(32) s[16];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_short  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __ECL_ANON_STRUCT__ struct{ ecl_short  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __ECL_ANON_STRUCT__ struct{ ecl_short8 lo, hi; };
#endif
#if defined( __ECL_SHORT2__)
    __ecl_short2     v2[8];
#endif
#if defined( __ECL_SHORT4__)
    __ecl_short4     v4[4];
#endif
#if defined( __ECL_SHORT8__ )
    __ecl_short8     v8[2];
#endif
#if defined( __ECL_SHORT16__ )
    __ecl_short16    v16;
#endif
}ecl_short16;


/* ---- ecl_ushortn ---- */
typedef union
{
    ecl_ushort  ECL_ALIGNED(4) s[2];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_ushort  x, y; };
   __ECL_ANON_STRUCT__ struct{ ecl_ushort  s0, s1; };
   __ECL_ANON_STRUCT__ struct{ ecl_ushort  lo, hi; };
#endif
#if defined( __ECL_USHORT2__)
    __ecl_ushort2     v2;
#endif
}ecl_ushort2;

typedef union
{
    ecl_ushort  ECL_ALIGNED(8) s[4];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_ushort  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_ushort  s0, s1, s2, s3; };
   __ECL_ANON_STRUCT__ struct{ ecl_ushort2 lo, hi; };
#endif
#if defined( __ECL_USHORT2__)
    __ecl_ushort2     v2[2];
#endif
#if defined( __ECL_USHORT4__)
    __ecl_ushort4     v4;
#endif
}ecl_ushort4;

/* ecl_ushort3 is identical in size, alignment and behavior to ecl_ushort4. See section 6.1.5. */
typedef  ecl_ushort4  ecl_ushort3;

typedef union
{
    ecl_ushort   ECL_ALIGNED(16) s[8];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_ushort  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_ushort  s0, s1, s2, s3, s4, s5, s6, s7; };
   __ECL_ANON_STRUCT__ struct{ ecl_ushort4 lo, hi; };
#endif
#if defined( __ECL_USHORT2__)
    __ecl_ushort2     v2[4];
#endif
#if defined( __ECL_USHORT4__)
    __ecl_ushort4     v4[2];
#endif
#if defined( __ECL_USHORT8__ )
    __ecl_ushort8     v8;
#endif
}ecl_ushort8;

typedef union
{
    ecl_ushort  ECL_ALIGNED(32) s[16];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_ushort  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __ECL_ANON_STRUCT__ struct{ ecl_ushort  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __ECL_ANON_STRUCT__ struct{ ecl_ushort8 lo, hi; };
#endif
#if defined( __ECL_USHORT2__)
    __ecl_ushort2     v2[8];
#endif
#if defined( __ECL_USHORT4__)
    __ecl_ushort4     v4[4];
#endif
#if defined( __ECL_USHORT8__ )
    __ecl_ushort8     v8[2];
#endif
#if defined( __ECL_USHORT16__ )
    __ecl_ushort16    v16;
#endif
}ecl_ushort16;


/* ---- ecl_halfn ---- */
typedef union
{
    ecl_half  ECL_ALIGNED(4) s[2];
#if __ECL_HAS_ANON_STRUCT__
    __ECL_ANON_STRUCT__ struct{ ecl_half  x, y; };
    __ECL_ANON_STRUCT__ struct{ ecl_half  s0, s1; };
    __ECL_ANON_STRUCT__ struct{ ecl_half  lo, hi; };
#endif
#if defined( __ECL_HALF2__)
    __ecl_half2     v2;
#endif
}ecl_half2;

typedef union
{
    ecl_half  ECL_ALIGNED(8) s[4];
#if __ECL_HAS_ANON_STRUCT__
    __ECL_ANON_STRUCT__ struct{ ecl_half  x, y, z, w; };
    __ECL_ANON_STRUCT__ struct{ ecl_half  s0, s1, s2, s3; };
    __ECL_ANON_STRUCT__ struct{ ecl_half2 lo, hi; };
#endif
#if defined( __ECL_HALF2__)
    __ecl_half2     v2[2];
#endif
#if defined( __ECL_HALF4__)
    __ecl_half4     v4;
#endif
}ecl_half4;

/* ecl_half3 is identical in size, alignment and behavior to ecl_half4. See section 6.1.5. */
typedef  ecl_half4  ecl_half3;

typedef union
{
    ecl_half   ECL_ALIGNED(16) s[8];
#if __ECL_HAS_ANON_STRUCT__
    __ECL_ANON_STRUCT__ struct{ ecl_half  x, y, z, w; };
    __ECL_ANON_STRUCT__ struct{ ecl_half  s0, s1, s2, s3, s4, s5, s6, s7; };
    __ECL_ANON_STRUCT__ struct{ ecl_half4 lo, hi; };
#endif
#if defined( __ECL_HALF2__)
    __ecl_half2     v2[4];
#endif
#if defined( __ECL_HALF4__)
    __ecl_half4     v4[2];
#endif
#if defined( __ECL_HALF8__ )
    __ecl_half8     v8;
#endif
}ecl_half8;

typedef union
{
    ecl_half  ECL_ALIGNED(32) s[16];
#if __ECL_HAS_ANON_STRUCT__
    __ECL_ANON_STRUCT__ struct{ ecl_half  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
    __ECL_ANON_STRUCT__ struct{ ecl_half  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
    __ECL_ANON_STRUCT__ struct{ ecl_half8 lo, hi; };
#endif
#if defined( __ECL_HALF2__)
    __ecl_half2     v2[8];
#endif
#if defined( __ECL_HALF4__)
    __ecl_half4     v4[4];
#endif
#if defined( __ECL_HALF8__ )
    __ecl_half8     v8[2];
#endif
#if defined( __ECL_HALF16__ )
    __ecl_half16    v16;
#endif
}ecl_half16;

/* ---- ecl_intn ---- */
typedef union
{
    ecl_int  ECL_ALIGNED(8) s[2];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_int  x, y; };
   __ECL_ANON_STRUCT__ struct{ ecl_int  s0, s1; };
   __ECL_ANON_STRUCT__ struct{ ecl_int  lo, hi; };
#endif
#if defined( __ECL_INT2__)
    __ecl_int2     v2;
#endif
}ecl_int2;

typedef union
{
    ecl_int  ECL_ALIGNED(16) s[4];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_int  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_int  s0, s1, s2, s3; };
   __ECL_ANON_STRUCT__ struct{ ecl_int2 lo, hi; };
#endif
#if defined( __ECL_INT2__)
    __ecl_int2     v2[2];
#endif
#if defined( __ECL_INT4__)
    __ecl_int4     v4;
#endif
}ecl_int4;

/* ecl_int3 is identical in size, alignment and behavior to ecl_int4. See section 6.1.5. */
typedef  ecl_int4  ecl_int3;

typedef union
{
    ecl_int   ECL_ALIGNED(32) s[8];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_int  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_int  s0, s1, s2, s3, s4, s5, s6, s7; };
   __ECL_ANON_STRUCT__ struct{ ecl_int4 lo, hi; };
#endif
#if defined( __ECL_INT2__)
    __ecl_int2     v2[4];
#endif
#if defined( __ECL_INT4__)
    __ecl_int4     v4[2];
#endif
#if defined( __ECL_INT8__ )
    __ecl_int8     v8;
#endif
}ecl_int8;

typedef union
{
    ecl_int  ECL_ALIGNED(64) s[16];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_int  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __ECL_ANON_STRUCT__ struct{ ecl_int  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __ECL_ANON_STRUCT__ struct{ ecl_int8 lo, hi; };
#endif
#if defined( __ECL_INT2__)
    __ecl_int2     v2[8];
#endif
#if defined( __ECL_INT4__)
    __ecl_int4     v4[4];
#endif
#if defined( __ECL_INT8__ )
    __ecl_int8     v8[2];
#endif
#if defined( __ECL_INT16__ )
    __ecl_int16    v16;
#endif
}ecl_int16;


/* ---- ecl_uintn ---- */
typedef union
{
    ecl_uint  ECL_ALIGNED(8) s[2];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_uint  x, y; };
   __ECL_ANON_STRUCT__ struct{ ecl_uint  s0, s1; };
   __ECL_ANON_STRUCT__ struct{ ecl_uint  lo, hi; };
#endif
#if defined( __ECL_UINT2__)
    __ecl_uint2     v2;
#endif
}ecl_uint2;

typedef union
{
    ecl_uint  ECL_ALIGNED(16) s[4];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_uint  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_uint  s0, s1, s2, s3; };
   __ECL_ANON_STRUCT__ struct{ ecl_uint2 lo, hi; };
#endif
#if defined( __ECL_UINT2__)
    __ecl_uint2     v2[2];
#endif
#if defined( __ECL_UINT4__)
    __ecl_uint4     v4;
#endif
}ecl_uint4;

/* ecl_uint3 is identical in size, alignment and behavior to ecl_uint4. See section 6.1.5. */
typedef  ecl_uint4  ecl_uint3;

typedef union
{
    ecl_uint   ECL_ALIGNED(32) s[8];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_uint  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_uint  s0, s1, s2, s3, s4, s5, s6, s7; };
   __ECL_ANON_STRUCT__ struct{ ecl_uint4 lo, hi; };
#endif
#if defined( __ECL_UINT2__)
    __ecl_uint2     v2[4];
#endif
#if defined( __ECL_UINT4__)
    __ecl_uint4     v4[2];
#endif
#if defined( __ECL_UINT8__ )
    __ecl_uint8     v8;
#endif
}ecl_uint8;

typedef union
{
    ecl_uint  ECL_ALIGNED(64) s[16];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_uint  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __ECL_ANON_STRUCT__ struct{ ecl_uint  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __ECL_ANON_STRUCT__ struct{ ecl_uint8 lo, hi; };
#endif
#if defined( __ECL_UINT2__)
    __ecl_uint2     v2[8];
#endif
#if defined( __ECL_UINT4__)
    __ecl_uint4     v4[4];
#endif
#if defined( __ECL_UINT8__ )
    __ecl_uint8     v8[2];
#endif
#if defined( __ECL_UINT16__ )
    __ecl_uint16    v16;
#endif
}ecl_uint16;

/* ---- ecl_longn ---- */
typedef union
{
    ecl_long  ECL_ALIGNED(16) s[2];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_long  x, y; };
   __ECL_ANON_STRUCT__ struct{ ecl_long  s0, s1; };
   __ECL_ANON_STRUCT__ struct{ ecl_long  lo, hi; };
#endif
#if defined( __ECL_LONG2__)
    __ecl_long2     v2;
#endif
}ecl_long2;

typedef union
{
    ecl_long  ECL_ALIGNED(32) s[4];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_long  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_long  s0, s1, s2, s3; };
   __ECL_ANON_STRUCT__ struct{ ecl_long2 lo, hi; };
#endif
#if defined( __ECL_LONG2__)
    __ecl_long2     v2[2];
#endif
#if defined( __ECL_LONG4__)
    __ecl_long4     v4;
#endif
}ecl_long4;

/* ecl_long3 is identical in size, alignment and behavior to ecl_long4. See section 6.1.5. */
typedef  ecl_long4  ecl_long3;

typedef union
{
    ecl_long   ECL_ALIGNED(64) s[8];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_long  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_long  s0, s1, s2, s3, s4, s5, s6, s7; };
   __ECL_ANON_STRUCT__ struct{ ecl_long4 lo, hi; };
#endif
#if defined( __ECL_LONG2__)
    __ecl_long2     v2[4];
#endif
#if defined( __ECL_LONG4__)
    __ecl_long4     v4[2];
#endif
#if defined( __ECL_LONG8__ )
    __ecl_long8     v8;
#endif
}ecl_long8;

typedef union
{
    ecl_long  ECL_ALIGNED(128) s[16];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_long  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __ECL_ANON_STRUCT__ struct{ ecl_long  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __ECL_ANON_STRUCT__ struct{ ecl_long8 lo, hi; };
#endif
#if defined( __ECL_LONG2__)
    __ecl_long2     v2[8];
#endif
#if defined( __ECL_LONG4__)
    __ecl_long4     v4[4];
#endif
#if defined( __ECL_LONG8__ )
    __ecl_long8     v8[2];
#endif
#if defined( __ECL_LONG16__ )
    __ecl_long16    v16;
#endif
}ecl_long16;


/* ---- ecl_ulongn ---- */
typedef union
{
    ecl_ulong  ECL_ALIGNED(16) s[2];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_ulong  x, y; };
   __ECL_ANON_STRUCT__ struct{ ecl_ulong  s0, s1; };
   __ECL_ANON_STRUCT__ struct{ ecl_ulong  lo, hi; };
#endif
#if defined( __ECL_ULONG2__)
    __ecl_ulong2     v2;
#endif
}ecl_ulong2;

typedef union
{
    ecl_ulong  ECL_ALIGNED(32) s[4];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_ulong  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_ulong  s0, s1, s2, s3; };
   __ECL_ANON_STRUCT__ struct{ ecl_ulong2 lo, hi; };
#endif
#if defined( __ECL_ULONG2__)
    __ecl_ulong2     v2[2];
#endif
#if defined( __ECL_ULONG4__)
    __ecl_ulong4     v4;
#endif
}ecl_ulong4;

/* ecl_ulong3 is identical in size, alignment and behavior to ecl_ulong4. See section 6.1.5. */
typedef  ecl_ulong4  ecl_ulong3;

typedef union
{
    ecl_ulong   ECL_ALIGNED(64) s[8];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_ulong  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_ulong  s0, s1, s2, s3, s4, s5, s6, s7; };
   __ECL_ANON_STRUCT__ struct{ ecl_ulong4 lo, hi; };
#endif
#if defined( __ECL_ULONG2__)
    __ecl_ulong2     v2[4];
#endif
#if defined( __ECL_ULONG4__)
    __ecl_ulong4     v4[2];
#endif
#if defined( __ECL_ULONG8__ )
    __ecl_ulong8     v8;
#endif
}ecl_ulong8;

typedef union
{
    ecl_ulong  ECL_ALIGNED(128) s[16];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_ulong  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __ECL_ANON_STRUCT__ struct{ ecl_ulong  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __ECL_ANON_STRUCT__ struct{ ecl_ulong8 lo, hi; };
#endif
#if defined( __ECL_ULONG2__)
    __ecl_ulong2     v2[8];
#endif
#if defined( __ECL_ULONG4__)
    __ecl_ulong4     v4[4];
#endif
#if defined( __ECL_ULONG8__ )
    __ecl_ulong8     v8[2];
#endif
#if defined( __ECL_ULONG16__ )
    __ecl_ulong16    v16;
#endif
}ecl_ulong16;


/* --- ecl_floatn ---- */

typedef union
{
    ecl_float  ECL_ALIGNED(8) s[2];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_float  x, y; };
   __ECL_ANON_STRUCT__ struct{ ecl_float  s0, s1; };
   __ECL_ANON_STRUCT__ struct{ ecl_float  lo, hi; };
#endif
#if defined( __ECL_FLOAT2__)
    __ecl_float2     v2;
#endif
}ecl_float2;

typedef union
{
    ecl_float  ECL_ALIGNED(16) s[4];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_float   x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_float   s0, s1, s2, s3; };
   __ECL_ANON_STRUCT__ struct{ ecl_float2  lo, hi; };
#endif
#if defined( __ECL_FLOAT2__)
    __ecl_float2     v2[2];
#endif
#if defined( __ECL_FLOAT4__)
    __ecl_float4     v4;
#endif
}ecl_float4;

/* ecl_float3 is identical in size, alignment and behavior to ecl_float4. See section 6.1.5. */
typedef  ecl_float4  ecl_float3;

typedef union
{
    ecl_float   ECL_ALIGNED(32) s[8];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_float   x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_float   s0, s1, s2, s3, s4, s5, s6, s7; };
   __ECL_ANON_STRUCT__ struct{ ecl_float4  lo, hi; };
#endif
#if defined( __ECL_FLOAT2__)
    __ecl_float2     v2[4];
#endif
#if defined( __ECL_FLOAT4__)
    __ecl_float4     v4[2];
#endif
#if defined( __ECL_FLOAT8__ )
    __ecl_float8     v8;
#endif
}ecl_float8;

typedef union
{
    ecl_float  ECL_ALIGNED(64) s[16];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_float  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __ECL_ANON_STRUCT__ struct{ ecl_float  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __ECL_ANON_STRUCT__ struct{ ecl_float8 lo, hi; };
#endif
#if defined( __ECL_FLOAT2__)
    __ecl_float2     v2[8];
#endif
#if defined( __ECL_FLOAT4__)
    __ecl_float4     v4[4];
#endif
#if defined( __ECL_FLOAT8__ )
    __ecl_float8     v8[2];
#endif
#if defined( __ECL_FLOAT16__ )
    __ecl_float16    v16;
#endif
}ecl_float16;

/* --- ecl_doublen ---- */

typedef union
{
    ecl_double  ECL_ALIGNED(16) s[2];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_double  x, y; };
   __ECL_ANON_STRUCT__ struct{ ecl_double s0, s1; };
   __ECL_ANON_STRUCT__ struct{ ecl_double lo, hi; };
#endif
#if defined( __ECL_DOUBLE2__)
    __ecl_double2     v2;
#endif
}ecl_double2;

typedef union
{
    ecl_double  ECL_ALIGNED(32) s[4];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_double  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_double  s0, s1, s2, s3; };
   __ECL_ANON_STRUCT__ struct{ ecl_double2 lo, hi; };
#endif
#if defined( __ECL_DOUBLE2__)
    __ecl_double2     v2[2];
#endif
#if defined( __ECL_DOUBLE4__)
    __ecl_double4     v4;
#endif
}ecl_double4;

/* ecl_double3 is identical in size, alignment and behavior to ecl_double4. See section 6.1.5. */
typedef  ecl_double4  ecl_double3;

typedef union
{
    ecl_double   ECL_ALIGNED(64) s[8];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_double  x, y, z, w; };
   __ECL_ANON_STRUCT__ struct{ ecl_double  s0, s1, s2, s3, s4, s5, s6, s7; };
   __ECL_ANON_STRUCT__ struct{ ecl_double4 lo, hi; };
#endif
#if defined( __ECL_DOUBLE2__)
    __ecl_double2     v2[4];
#endif
#if defined( __ECL_DOUBLE4__)
    __ecl_double4     v4[2];
#endif
#if defined( __ECL_DOUBLE8__ )
    __ecl_double8     v8;
#endif
}ecl_double8;

typedef union
{
    ecl_double  ECL_ALIGNED(128) s[16];
#if __ECL_HAS_ANON_STRUCT__
   __ECL_ANON_STRUCT__ struct{ ecl_double  x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf; };
   __ECL_ANON_STRUCT__ struct{ ecl_double  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF; };
   __ECL_ANON_STRUCT__ struct{ ecl_double8 lo, hi; };
#endif
#if defined( __ECL_DOUBLE2__)
    __ecl_double2     v2[8];
#endif
#if defined( __ECL_DOUBLE4__)
    __ecl_double4     v4[4];
#endif
#if defined( __ECL_DOUBLE8__ )
    __ecl_double8     v8[2];
#endif
#if defined( __ECL_DOUBLE16__ )
    __ecl_double16    v16;
#endif
}ecl_double16;

/* Macro to facilitate debugging
 * Usage:
 *   Place ECL_PROGRAM_STRING_DEBUG_INFO on the line before the first line of your source.
 *   The first line ends with:   ECL_PROGRAM_STRING_DEBUG_INFO \"
 *   Each line thereafter of OpenCL C source must end with: \n\
 *   The last line ends in ";
 *
 *   Example:
 *
 *   const char *my_program = ECL_PROGRAM_STRING_DEBUG_INFO "\
 *   kernel void foo( int a, float * b )             \n\
 *   {                                               \n\
 *      // my comment                                \n\
 *      *b[ get_global_id(0)] = a;                   \n\
 *   }                                               \n\
 *   ";
 *
 * This should correctly set up the line, (column) and file information for your source
 * string so you can do source level debugging.
 */
#define  __ECL_STRINGIFY( _x )               # _x
#define  _ECL_STRINGIFY( _x )                __ECL_STRINGIFY( _x )
#define  ECL_PROGRAM_STRING_DEBUG_INFO       "#line "  _ECL_STRINGIFY(__LINE__) " \"" __FILE__ "\" \n\n"

#ifdef __cplusplus
}
#endif

#undef __ECL_HAS_ANON_STRUCT__
#undef __ECL_ANON_STRUCT__
#if defined( _WIN32) && defined(_MSC_VER)
    #if _MSC_VER >=1500
    #pragma warning( pop )
    #endif
#endif

#endif  /* __ELCORE_ECL_PLATFORM_H  */
