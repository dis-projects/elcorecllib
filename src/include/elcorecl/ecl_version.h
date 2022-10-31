// Copyright 2018-2019 RnD Center "ELVEES", JSC
/*******************************************************************************
 * Copyright (c) 2018 The Khronos Group Inc.
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
 ******************************************************************************/

#ifndef __ELCORE_ECL_VERSION_H
#define __ELCORE_ECL_VERSION_H

/* Detect which version to target */
#if !defined(ECL_TARGET_OPENCL_VERSION)
#define ECL_TARGET_OPENCL_VERSION 220
#endif
#if ECL_TARGET_OPENCL_VERSION != 100 && \
    ECL_TARGET_OPENCL_VERSION != 110 && \
    ECL_TARGET_OPENCL_VERSION != 120 && \
    ECL_TARGET_OPENCL_VERSION != 200 && \
    ECL_TARGET_OPENCL_VERSION != 210 && \
    ECL_TARGET_OPENCL_VERSION != 220
#pragma message("ecl_version: ECL_TARGET_OPENCL_VERSION is not a valid value (100, 110, 120, 200, 210, 220). Defaulting to 220 (OpenCL 2.2)")
#undef ECL_TARGET_OPENCL_VERSION
#define ECL_TARGET_OPENCL_VERSION 220
#endif


/* OpenCL Version */
#if ECL_TARGET_OPENCL_VERSION >= 220 && !defined(ECL_VERSION_2_2)
#define ECL_VERSION_2_2  1
#endif
#if ECL_TARGET_OPENCL_VERSION >= 210 && !defined(ECL_VERSION_2_1)
#define ECL_VERSION_2_1  1
#endif
#if ECL_TARGET_OPENCL_VERSION >= 200 && !defined(ECL_VERSION_2_0)
#define ECL_VERSION_2_0  1
#endif
#if ECL_TARGET_OPENCL_VERSION >= 120 && !defined(ECL_VERSION_1_2)
#define ECL_VERSION_1_2  1
#endif
#if ECL_TARGET_OPENCL_VERSION >= 110 && !defined(ECL_VERSION_1_1)
#define ECL_VERSION_1_1  1
#endif
#if ECL_TARGET_OPENCL_VERSION >= 100 && !defined(ECL_VERSION_1_0)
#define ECL_VERSION_1_0  1
#endif

/* Allow deprecated APIs for older OpenCL versions. */
#if ECL_TARGET_OPENCL_VERSION <= 210 && !defined(ECL_USE_DEPRECATED_OPENCL_2_1_APIS)
#define ECL_USE_DEPRECATED_OPENCL_2_1_APIS
#endif
#if ECL_TARGET_OPENCL_VERSION <= 200 && !defined(ECL_USE_DEPRECATED_OPENCL_2_0_APIS)
#define ECL_USE_DEPRECATED_OPENCL_2_0_APIS
#endif
#if ECL_TARGET_OPENCL_VERSION <= 120 && !defined(ECL_USE_DEPRECATED_OPENCL_1_2_APIS)
#define ECL_USE_DEPRECATED_OPENCL_1_2_APIS
#endif
#if ECL_TARGET_OPENCL_VERSION <= 110 && !defined(ECL_USE_DEPRECATED_OPENCL_1_1_APIS)
#define ECL_USE_DEPRECATED_OPENCL_1_1_APIS
#endif
#if ECL_TARGET_OPENCL_VERSION <= 100 && !defined(ECL_USE_DEPRECATED_OPENCL_1_0_APIS)
#define ECL_USE_DEPRECATED_OPENCL_1_0_APIS
#endif

#endif  /* __ELCORE_ECL_VERSION_H */
