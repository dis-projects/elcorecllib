// Copyright 2018-2019 RnD Center "ELVEES", JSC
/**********************************************************************************
 * Copyright (c) 2008-2015 The Khronos Group Inc.
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

/* $Revision: 11708 $ on $Date: 2010-06-13 23:36:24 -0700 (Sun, 13 Jun 2010) $ */

#ifndef __ELCORE_ECL_DX9_MEDIA_SHARING_H
#define __ELCORE_ECL_DX9_MEDIA_SHARING_H

#include <elcorecl/ecl.h>
#include <elcorecl/ecl_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* ecl_khr_dx9_media_sharing                                                   */
#define ecl_khr_dx9_media_sharing 1

typedef ecl_uint             ecl_dx9_media_adapter_type_khr;
typedef ecl_uint             ecl_dx9_media_adapter_set_khr;

#if defined(_WIN32)
#include <d3d9.h>
typedef struct _ecl_dx9_surface_info_khr
{
    IDirect3DSurface9 *resource;
    HANDLE shared_handle;
} ecl_dx9_surface_info_khr;
#endif


/******************************************************************************/

/* Error Codes */
#define ECL_INVALID_DX9_MEDIA_ADAPTER_KHR                -1010
#define ECL_INVALID_DX9_MEDIA_SURFACE_KHR                -1011
#define ECL_DX9_MEDIA_SURFACE_ALREADY_ACQUIRED_KHR       -1012
#define ECL_DX9_MEDIA_SURFACE_NOT_ACQUIRED_KHR           -1013

/* ecl_media_adapter_type_khr */
#define ECL_ADAPTER_D3D9_KHR                              0x2020
#define ECL_ADAPTER_D3D9EX_KHR                            0x2021
#define ECL_ADAPTER_DXVA_KHR                              0x2022

/* ecl_media_adapter_set_khr */
#define ECL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR   0x2023
#define ECL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR         0x2024

/* ecl_context_info */
#define ECL_CONTEXT_ADAPTER_D3D9_KHR                      0x2025
#define ECL_CONTEXT_ADAPTER_D3D9EX_KHR                    0x2026
#define ECL_CONTEXT_ADAPTER_DXVA_KHR                      0x2027

/* ecl_mem_info */
#define ECL_MEM_DX9_MEDIA_ADAPTER_TYPE_KHR                0x2028
#define ECL_MEM_DX9_MEDIA_SURFACE_INFO_KHR                0x2029

/* ecl_image_info */
#define ECL_IMAGE_DX9_MEDIA_PLANE_KHR                     0x202A

/* ecl_command_type */
#define ECL_COMMAND_ACQUIRE_DX9_MEDIA_SURFACES_KHR        0x202B
#define ECL_COMMAND_RELEASE_DX9_MEDIA_SURFACES_KHR        0x202C

/******************************************************************************/

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclGetDeviceIDsFromDX9MediaAdapterKHR_fn)(
    ecl_platform_id                   platform,
    ecl_uint                          num_media_adapters,
    ecl_dx9_media_adapter_type_khr *  media_adapter_type,
    void *                           media_adapters,
    ecl_dx9_media_adapter_set_khr     media_adapter_set,
    ecl_uint                          num_entries,
    ecl_device_id *                   devices,
    ecl_uint *                        num_devices) ECL_API_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_mem (ECL_API_CALL *eclCreateFromDX9MediaSurfaceKHR_fn)(
    ecl_context                    context,
    ecl_mem_flags                  flags,
    ecl_dx9_media_adapter_type_khr adapter_type,
    void *                        surface_info,
    ecl_uint                       plane,
    ecl_int *                      errcode_ret) ECL_API_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclEnqueueAcquireDX9MediaSurfacesKHR_fn)(
    ecl_command_queue command_queue,
    ecl_uint          num_objects,
    const ecl_mem *   mem_objects,
    ecl_uint          num_events_in_wait_list,
    const ecl_event * event_wait_list,
    ecl_event *       event) ECL_API_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclEnqueueReleaseDX9MediaSurfacesKHR_fn)(
    ecl_command_queue command_queue,
    ecl_uint          num_objects,
    const ecl_mem *   mem_objects,
    ecl_uint          num_events_in_wait_list,
    const ecl_event * event_wait_list,
    ecl_event *       event) ECL_API_SUFFIX__VERSION_1_2;

#ifdef __cplusplus
}
#endif

#endif  /* __ELCORE_ECL_DX9_MEDIA_SHARING_H */
