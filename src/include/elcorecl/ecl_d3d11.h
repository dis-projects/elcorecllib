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

#ifndef __ELCORE_ECL_D3D11_H
#define __ELCORE_ECL_D3D11_H

#include <d3d11.h>
#include <elcorecl/ecl.h>
#include <elcorecl/ecl_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * ecl_khr_d3d11_sharing                                                       */
#define ecl_khr_d3d11_sharing 1

typedef ecl_uint ecl_d3d11_device_source_khr;
typedef ecl_uint ecl_d3d11_device_set_khr;

/******************************************************************************/

/* Error Codes */
#define ECL_INVALID_D3D11_DEVICE_KHR                  -1006
#define ECL_INVALID_D3D11_RESOURCE_KHR                -1007
#define ECL_D3D11_RESOURCE_ALREADY_ACQUIRED_KHR       -1008
#define ECL_D3D11_RESOURCE_NOT_ACQUIRED_KHR           -1009

/* ecl_d3d11_device_source */
#define ECL_D3D11_DEVICE_KHR                          0x4019
#define ECL_D3D11_DXGI_ADAPTER_KHR                    0x401A

/* ecl_d3d11_device_set */
#define ECL_PREFERRED_DEVICES_FOR_D3D11_KHR           0x401B
#define ECL_ALL_DEVICES_FOR_D3D11_KHR                 0x401C

/* ecl_context_info */
#define ECL_CONTEXT_D3D11_DEVICE_KHR                  0x401D
#define ECL_CONTEXT_D3D11_PREFER_SHARED_RESOURCES_KHR 0x402D

/* ecl_mem_info */
#define ECL_MEM_D3D11_RESOURCE_KHR                    0x401E

/* ecl_image_info */
#define ECL_IMAGE_D3D11_SUBRESOURCE_KHR               0x401F

/* ecl_command_type */
#define ECL_COMMAND_ACQUIRE_D3D11_OBJECTS_KHR         0x4020
#define ECL_COMMAND_RELEASE_D3D11_OBJECTS_KHR         0x4021

/******************************************************************************/

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclGetDeviceIDsFromD3D11KHR_fn)(
    ecl_platform_id             platform,
    ecl_d3d11_device_source_khr d3d_device_source,
    void *                     d3d_object,
    ecl_d3d11_device_set_khr    d3d_device_set,
    ecl_uint                    num_entries,
    ecl_device_id *             devices,
    ecl_uint *                  num_devices) ECL_API_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_mem (ECL_API_CALL *eclCreateFromD3D11BufferKHR_fn)(
    ecl_context     context,
    ecl_mem_flags   flags,
    ID3D11Buffer * resource,
    ecl_int *       errcode_ret) ECL_API_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_mem (ECL_API_CALL *eclCreateFromD3D11Texture2DKHR_fn)(
    ecl_context        context,
    ecl_mem_flags      flags,
    ID3D11Texture2D * resource,
    UINT              subresource,
    ecl_int *          errcode_ret) ECL_API_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_mem (ECL_API_CALL *eclCreateFromD3D11Texture3DKHR_fn)(
    ecl_context        context,
    ecl_mem_flags      flags,
    ID3D11Texture3D * resource,
    UINT              subresource,
    ecl_int *          errcode_ret) ECL_API_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclEnqueueAcquireD3D11ObjectsKHR_fn)(
    ecl_command_queue command_queue,
    ecl_uint          num_objects,
    const ecl_mem *   mem_objects,
    ecl_uint          num_events_in_wait_list,
    const ecl_event * event_wait_list,
    ecl_event *       event) ECL_API_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclEnqueueReleaseD3D11ObjectsKHR_fn)(
    ecl_command_queue command_queue,
    ecl_uint          num_objects,
    const ecl_mem *   mem_objects,
    ecl_uint          num_events_in_wait_list,
    const ecl_event * event_wait_list,
    ecl_event *       event) ECL_API_SUFFIX__VERSION_1_2;

#ifdef __cplusplus
}
#endif

#endif  /* __ELCORE_ECL_D3D11_H */
