// Copyright 2018-2019 RnD Center "ELVEES", JSC
/**********************************************************************************
 * Copyright (c) 2008-2016 The Khronos Group Inc.
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
/*****************************************************************************\

Copyright (c) 2013-2016 Intel Corporation All Rights Reserved.

THESE MATERIALS ARE PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR ITS
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THESE
MATERIALS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

File Name: ecl_va_api_media_sharing_intel.h

Abstract:

Notes:

\*****************************************************************************/


#ifndef __ELCORE_ECL_VA_API_MEDIA_SHARING_INTEL_H
#define __ELCORE_ECL_VA_API_MEDIA_SHARING_INTEL_H

#include <elcorecl/ecl.h>
#include <elcorecl/ecl_platform.h>
#include <va/va.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************
* ecl_intel_va_api_media_sharing extension *
*******************************************/

#define ecl_intel_va_api_media_sharing 1

/* error codes */
#define ECL_INVALID_VA_API_MEDIA_ADAPTER_INTEL               -1098
#define ECL_INVALID_VA_API_MEDIA_SURFACE_INTEL               -1099
#define ECL_VA_API_MEDIA_SURFACE_ALREADY_ACQUIRED_INTEL      -1100
#define ECL_VA_API_MEDIA_SURFACE_NOT_ACQUIRED_INTEL          -1101

/* ecl_va_api_device_source_intel */
#define ECL_VA_API_DISPLAY_INTEL                             0x4094

/* ecl_va_api_device_set_intel */
#define ECL_PREFERRED_DEVICES_FOR_VA_API_INTEL               0x4095
#define ECL_ALL_DEVICES_FOR_VA_API_INTEL                     0x4096

/* ecl_context_info */
#define ECL_CONTEXT_VA_API_DISPLAY_INTEL                     0x4097

/* ecl_mem_info */
#define ECL_MEM_VA_API_MEDIA_SURFACE_INTEL                   0x4098

/* ecl_image_info */
#define ECL_IMAGE_VA_API_PLANE_INTEL                         0x4099

/* ecl_command_type */
#define ECL_COMMAND_ACQUIRE_VA_API_MEDIA_SURFACES_INTEL      0x409A
#define ECL_COMMAND_RELEASE_VA_API_MEDIA_SURFACES_INTEL      0x409B

typedef ecl_uint ecl_va_api_device_source_intel;
typedef ecl_uint ecl_va_api_device_set_intel;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetDeviceIDsFromVA_APIMediaAdapterINTEL(
    ecl_platform_id                /* platform */,
    ecl_va_api_device_source_intel /* media_adapter_type */,
    void*                         /* media_adapter */,
    ecl_va_api_device_set_intel    /* media_adapter_set */,
    ecl_uint                       /* num_entries */,
    ecl_device_id*                 /* devices */,
    ecl_uint*                      /* num_devices */) ECL_EXT_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL * eclGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)(
    ecl_platform_id                /* platform */,
    ecl_va_api_device_source_intel /* media_adapter_type */,
    void*                         /* media_adapter */,
    ecl_va_api_device_set_intel    /* media_adapter_set */,
    ecl_uint                       /* num_entries */,
    ecl_device_id*                 /* devices */,
    ecl_uint*                      /* num_devices */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateFromVA_APIMediaSurfaceINTEL(
    ecl_context                    /* context */,
    ecl_mem_flags                  /* flags */,
    VASurfaceID*                  /* surface */,
    ecl_uint                       /* plane */,
    ecl_int*                       /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_mem (ECL_API_CALL * eclCreateFromVA_APIMediaSurfaceINTEL_fn)(
    ecl_context                    /* context */,
    ecl_mem_flags                  /* flags */,
    VASurfaceID*                  /* surface */,
    ecl_uint                       /* plane */,
    ecl_int*                       /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueAcquireVA_APIMediaSurfacesINTEL(
    ecl_command_queue              /* command_queue */,
    ecl_uint                       /* num_objects */,
    const ecl_mem*                 /* mem_objects */,
    ecl_uint                       /* num_events_in_wait_list */,
    const ecl_event*               /* event_wait_list */,
    ecl_event*                     /* event */) ECL_EXT_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)(
    ecl_command_queue              /* command_queue */,
    ecl_uint                       /* num_objects */,
    const ecl_mem*                 /* mem_objects */,
    ecl_uint                       /* num_events_in_wait_list */,
    const ecl_event*               /* event_wait_list */,
    ecl_event*                     /* event */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueReleaseVA_APIMediaSurfacesINTEL(
    ecl_command_queue              /* command_queue */,
    ecl_uint                       /* num_objects */,
    const ecl_mem*                 /* mem_objects */,
    ecl_uint                       /* num_events_in_wait_list */,
    const ecl_event*               /* event_wait_list */,
    ecl_event*                     /* event */) ECL_EXT_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)(
    ecl_command_queue              /* command_queue */,
    ecl_uint                       /* num_objects */,
    const ecl_mem*                 /* mem_objects */,
    ecl_uint                       /* num_events_in_wait_list */,
    const ecl_event*               /* event_wait_list */,
    ecl_event*                     /* event */) ECL_EXT_SUFFIX__VERSION_1_2;

#ifdef __cplusplus
}
#endif

#endif  /* __ELCORE_ECL_VA_API_MEDIA_SHARING_INTEL_H */
