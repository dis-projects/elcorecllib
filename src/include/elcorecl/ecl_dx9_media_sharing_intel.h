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

File Name: ecl_dx9_media_sharing_intel.h

Abstract:

Notes:

\*****************************************************************************/

#ifndef __ELCORE_ECL_DX9_MEDIA_SHARING_INTEL_H
#define __ELCORE_ECL_DX9_MEDIA_SHARING_INTEL_H

#include <elcorecl/ecl.h>
#include <elcorecl/ecl_platform.h>
#include <d3d9.h>
#include <dxvahd.h>
#include <wtypes.h>
#include <d3d9types.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
* ecl_intel_dx9_media_sharing extension *
****************************************/

#define ecl_intel_dx9_media_sharing 1

typedef ecl_uint ecl_dx9_device_source_intel;
typedef ecl_uint ecl_dx9_device_set_intel;

/* error codes */
#define ECL_INVALID_DX9_DEVICE_INTEL                   -1010
#define ECL_INVALID_DX9_RESOURCE_INTEL                 -1011
#define ECL_DX9_RESOURCE_ALREADY_ACQUIRED_INTEL        -1012
#define ECL_DX9_RESOURCE_NOT_ACQUIRED_INTEL            -1013

/* ecl_dx9_device_source_intel */
#define ECL_D3D9_DEVICE_INTEL                          0x4022
#define ECL_D3D9EX_DEVICE_INTEL                        0x4070
#define ECL_DXVA_DEVICE_INTEL                          0x4071

/* ecl_dx9_device_set_intel */
#define ECL_PREFERRED_DEVICES_FOR_DX9_INTEL            0x4024
#define ECL_ALL_DEVICES_FOR_DX9_INTEL                  0x4025

/* ecl_context_info */
#define ECL_CONTEXT_D3D9_DEVICE_INTEL                  0x4026
#define ECL_CONTEXT_D3D9EX_DEVICE_INTEL                0x4072
#define ECL_CONTEXT_DXVA_DEVICE_INTEL                  0x4073

/* ecl_mem_info */
#define ECL_MEM_DX9_RESOURCE_INTEL                     0x4027
#define ECL_MEM_DX9_SHARED_HANDLE_INTEL                0x4074

/* ecl_image_info */
#define ECL_IMAGE_DX9_PLANE_INTEL                      0x4075

/* ecl_command_type */
#define ECL_COMMAND_ACQUIRE_DX9_OBJECTS_INTEL          0x402A
#define ECL_COMMAND_RELEASE_DX9_OBJECTS_INTEL          0x402B
/******************************************************************************/

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetDeviceIDsFromDX9INTEL(
    ecl_platform_id              /* platform */,
    ecl_dx9_device_source_intel  /* dx9_device_source */,
    void*                       /* dx9_object */,
    ecl_dx9_device_set_intel     /* dx9_device_set */,
    ecl_uint                     /* num_entries */,
    ecl_device_id*               /* devices */,
    ecl_uint*                    /* num_devices */) ECL_EXT_SUFFIX__VERSION_1_1;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL* eclGetDeviceIDsFromDX9INTEL_fn)(
    ecl_platform_id              /* platform */,
    ecl_dx9_device_source_intel  /* dx9_device_source */,
    void*                       /* dx9_object */,
    ecl_dx9_device_set_intel     /* dx9_device_set */,
    ecl_uint                     /* num_entries */,
    ecl_device_id*               /* devices */,
    ecl_uint*                    /* num_devices */) ECL_EXT_SUFFIX__VERSION_1_1;

extern ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateFromDX9MediaSurfaceINTEL(
    ecl_context                  /* context */,
    ecl_mem_flags                /* flags */,
    IDirect3DSurface9*          /* resource */,
    HANDLE                      /* sharedHandle */,
    UINT                        /* plane */,
    ecl_int*                     /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_1;

typedef ECL_API_ENTRY ecl_mem (ECL_API_CALL *eclCreateFromDX9MediaSurfaceINTEL_fn)(
    ecl_context                  /* context */,
    ecl_mem_flags                /* flags */,
    IDirect3DSurface9*          /* resource */,
    HANDLE                      /* sharedHandle */,
    UINT                        /* plane */,
    ecl_int*                     /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_1;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueAcquireDX9ObjectsINTEL(
    ecl_command_queue            /* command_queue */,
    ecl_uint                     /* num_objects */,
    const ecl_mem*               /* mem_objects */,
    ecl_uint                     /* num_events_in_wait_list */,
    const ecl_event*             /* event_wait_list */,
    ecl_event*                   /* event */) ECL_EXT_SUFFIX__VERSION_1_1;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclEnqueueAcquireDX9ObjectsINTEL_fn)(
    ecl_command_queue            /* command_queue */,
    ecl_uint                     /* num_objects */,
    const ecl_mem*               /* mem_objects */,
    ecl_uint                     /* num_events_in_wait_list */,
    const ecl_event*             /* event_wait_list */,
    ecl_event*                   /* event */) ECL_EXT_SUFFIX__VERSION_1_1;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueReleaseDX9ObjectsINTEL(
    ecl_command_queue            /* command_queue */,
    ecl_uint                     /* num_objects */,
    ecl_mem*                     /* mem_objects */,
    ecl_uint                     /* num_events_in_wait_list */,
    const ecl_event*             /* event_wait_list */,
    ecl_event*                   /* event */) ECL_EXT_SUFFIX__VERSION_1_1;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclEnqueueReleaseDX9ObjectsINTEL_fn)(
    ecl_command_queue            /* command_queue */,
    ecl_uint                     /* num_objects */,
    ecl_mem*                     /* mem_objects */,
    ecl_uint                     /* num_events_in_wait_list */,
    const ecl_event*             /* event_wait_list */,
    ecl_event*                   /* event */) ECL_EXT_SUFFIX__VERSION_1_1;

#ifdef __cplusplus
}
#endif

#endif  /* __ELCORE_ECL_DX9_MEDIA_SHARING_INTEL_H */
