// Copyright 2018-2019 RnD Center "ELVEES", JSC
/*******************************************************************************
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
 ******************************************************************************/

/* ecl_ext.h contains OpenCL extensions which don't have external */
/* (OpenGL, D3D) dependencies.                                   */

#ifndef __ELCORE_ECL_EXT_H
#define __ELCORE_ECL_EXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <elcorecl/ecl.h>

/* ecl_khr_fp64 extension - no extension #define since it has no functions  */
/* ECL_DEVICE_DOUBLE_FP_CONFIG is defined in ECL.h for OpenCL >= 120 */

#if ECL_TARGET_OPENCL_VERSION <= 110
#define ECL_DEVICE_DOUBLE_FP_CONFIG                       0x1032
#endif

/* ecl_khr_fp16 extension - no extension #define since it has no functions  */
#define ECL_DEVICE_HALF_FP_CONFIG                    0x1033

/* Memory object destruction
 *
 * Apple extension for use to manage externally allocated buffers used with ecl_mem objects with ECL_MEM_USE_HOST_PTR
 *
 * Registers a user callback function that will be called when the memory object is deleted and its resources
 * freed. Each call to eclSetMemObjectCallbackFn registers the specified user callback function on a callback
 * stack associated with memobj. The registered user callback functions are called in the reverse order in
 * which they were registered. The user callback functions are called and then the memory object is deleted
 * and its resources freed. This provides a mechanism for the application (and libraries) using memobj to be
 * notified when the memory referenced by host_ptr, specified when the memory object is created and used as
 * the storage bits for the memory object, can be reused or freed.
 *
 * The application may not call ElcoreCL api's with the ecl_mem object passed to the pfn_notify.
 *
 * Please check for the "ecl_APPLE_SetMemObjectDestructor" extension using eclGetDeviceInfo(ECL_DEVICE_EXTENSIONS)
 * before using.
 */
#define ecl_APPLE_SetMemObjectDestructor 1
ecl_int  ECL_API_ENTRY eclSetMemObjectDestructorAPPLE(  ecl_mem /* memobj */,
                                        void (* /*pfn_notify*/)( ecl_mem /* memobj */, void* /*user_data*/),
                                        void * /*user_data */ )             ECL_EXT_SUFFIX__VERSION_1_0;


/* Context Logging Functions
 *
 * The next three convenience functions are intended to be used as the pfn_notify parameter to eclCreateContext().
 * Please check for the "ecl_APPLE_ContextLoggingFunctions" extension using eclGetDeviceInfo(ECL_DEVICE_EXTENSIONS)
 * before using.
 *
 * eclLogMessagesToSystemLog fowards on all log messages to the Apple System Logger
 */
#define ecl_APPLE_ContextLoggingFunctions 1
extern void ECL_API_ENTRY eclLogMessagesToSystemLogAPPLE(  const char * /* errstr */,
                                            const void * /* private_info */,
                                            size_t       /* cb */,
                                            void *       /* user_data */ )  ECL_EXT_SUFFIX__VERSION_1_0;

/* eclLogMessagesToStdout sends all log messages to the file descriptor stdout */
extern void ECL_API_ENTRY eclLogMessagesToStdoutAPPLE(   const char * /* errstr */,
                                          const void * /* private_info */,
                                          size_t       /* cb */,
                                          void *       /* user_data */ )    ECL_EXT_SUFFIX__VERSION_1_0;

/* eclLogMessagesToStderr sends all log messages to the file descriptor stderr */
extern void ECL_API_ENTRY eclLogMessagesToStderrAPPLE(   const char * /* errstr */,
                                          const void * /* private_info */,
                                          size_t       /* cb */,
                                          void *       /* user_data */ )    ECL_EXT_SUFFIX__VERSION_1_0;


/************************
* ecl_khr_icd extension *
************************/
#define ecl_khr_icd 1

/* ecl_platform_info                                                        */
#define ECL_PLATFORM_ICD_SUFFIX_KHR                  0x0920

/* Additional Error Codes                                                  */
#define ECL_PLATFORM_NOT_FOUND_KHR                   -1001

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclIcdGetPlatformIDsKHR(ecl_uint          /* num_entries */,
                       ecl_platform_id * /* platforms */,
                       ecl_uint *        /* num_platforms */);

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclIcdGetPlatformIDsKHR_fn)(
    ecl_uint          /* num_entries */,
    ecl_platform_id * /* platforms */,
    ecl_uint *        /* num_platforms */);



/*******************************
 * ecl_khr_il_program extension *
 *******************************/
#define ecl_khr_il_program 1

/* New property to eclGetDeviceInfo for retrieving supported intermediate
 * languages
 */
#define ECL_DEVICE_IL_VERSION_KHR                    0x105B

/* New property to eclGetProgramInfo for retrieving for retrieving the IL of a
 * program
 */
#define ECL_PROGRAM_IL_KHR                           0x1169

extern ECL_API_ENTRY ecl_program
  ECL_API_CALL eclCreateProgramWithILKHR(
      ecl_context /* context */,
      const void * /* il */,
      size_t /* length */,
      ecl_int * /* errcode_ret */);

typedef ECL_API_ENTRY ecl_program
  (ECL_API_CALL *eclCreateProgramWithILKHR_fn)(
      ecl_context /* context */,
      const void * /* il */,
      size_t /* length */,
      ecl_int * /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_2;

/* Extension: ecl_khr_image2D_buffer
 *
 * This extension allows a 2D image to be created from a ecl_mem buffer without a copy.
 * The type associated with a 2D image created from a buffer in an OpenCL program is image2d_t.
 * Both the sampler and sampler-less read_image built-in functions are supported for 2D images
 * and 2D images created from a buffer.  Similarly, the write_image built-ins are also supported
 * for 2D images created from a buffer.
 *
 * When the 2D image from buffer is created, the eclient must specify the width,
 * height, image format (i.e. channel order and channel data type) and optionally the row pitch
 *
 * The pitch specified must be a multiple of ECL_DEVICE_IMAGE_PITCH_ALIGNMENT pixels.
 * The base address of the buffer must be aligned to ECL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT pixels.
 */

/**************************************
 * ecl_khr_initialize_memory extension *
 **************************************/

#define ECL_CONTEXT_MEMORY_INITIALIZE_KHR            0x2030


/**************************************
 * ecl_khr_terminate_context extension *
 **************************************/

#define ECL_DEVICE_TERMINATE_CAPABILITY_KHR          0x2031
#define ECL_CONTEXT_TERMINATE_KHR                    0x2032

#define ecl_khr_terminate_context 1
extern ECL_API_ENTRY ecl_int ECL_API_CALL eclTerminateContextKHR(ecl_context /* context */) ECL_EXT_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclTerminateContextKHR_fn)(ecl_context /* context */) ECL_EXT_SUFFIX__VERSION_1_2;


/*
 * Extension: ecl_khr_spir
 *
 * This extension adds support to create an OpenCL program object from a
 * Standard Portable Intermediate Representation (SPIR) instance
 */

#define ECL_DEVICE_SPIR_VERSIONS                     0x40E0
#define ECL_PROGRAM_BINARY_TYPE_INTERMEDIATE         0x40E1


/*****************************************
 * ecl_khr_create_command_queue extension *
 *****************************************/
#define ecl_khr_create_command_queue 1

typedef ecl_bitfield ecl_queue_properties_khr;

extern ECL_API_ENTRY ecl_command_queue ECL_API_CALL
eclCreateCommandQueueWithPropertiesKHR( ecl_context /* context */,
                                       ecl_device_id /* device */,
                                       const ecl_queue_properties_khr* /* properties */,
                                       ecl_int* /* errcode_ret */ ) ECL_EXT_SUFFIX__VERSION_1_2;
typedef ECL_API_ENTRY ecl_command_queue
(ECL_API_CALL *eclCreateCommandQueueWithPropertiesKHR_fn)( ecl_context /* context */,
                                                         ecl_device_id /* device */,
                                                         const ecl_queue_properties_khr* /* properties */,
                                                         ecl_int* /* errcode_ret */ ) ECL_EXT_SUFFIX__VERSION_1_2;


/******************************************
* ecl_nv_device_attribute_query extension *
******************************************/

/* ecl_nv_device_attribute_query extension - no extension #define since it has no functions */
#define ECL_DEVICE_COMPUTE_CAPABILITY_MAJOR_NV       0x4000
#define ECL_DEVICE_COMPUTE_CAPABILITY_MINOR_NV       0x4001
#define ECL_DEVICE_REGISTERS_PER_BLOCK_NV            0x4002
#define ECL_DEVICE_WARP_SIZE_NV                      0x4003
#define ECL_DEVICE_GPU_OVERLAP_NV                    0x4004
#define ECL_DEVICE_KERNEL_EXEC_TIMEOUT_NV            0x4005
#define ECL_DEVICE_INTEGRATED_MEMORY_NV              0x4006


/*********************************
* ecl_amd_device_attribute_query *
*********************************/

#define ECL_DEVICE_PROFILING_TIMER_OFFSET_AMD        0x4036


/*********************************
* ecl_arm_printf extension
*********************************/

#define ECL_PRINTF_CALLBACK_ARM                      0x40B0
#define ECL_PRINTF_BUFFERSIZE_ARM                    0x40B1


/***********************************
* ecl_ext_device_fission extension
***********************************/
#define ecl_ext_device_fission   1

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseDeviceEXT( ecl_device_id /*device*/ ) ECL_EXT_SUFFIX__VERSION_1_1;

typedef ECL_API_ENTRY ecl_int
(ECL_API_CALL *eclReleaseDeviceEXT_fn)( ecl_device_id /*device*/ ) ECL_EXT_SUFFIX__VERSION_1_1;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainDeviceEXT( ecl_device_id /*device*/ ) ECL_EXT_SUFFIX__VERSION_1_1;

typedef ECL_API_ENTRY ecl_int
(ECL_API_CALL *eclRetainDeviceEXT_fn)( ecl_device_id /*device*/ ) ECL_EXT_SUFFIX__VERSION_1_1;

typedef ecl_ulong  ecl_device_partition_property_ext;
extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclCreateSubDevicesEXT(  ecl_device_id /*in_device*/,
                        const ecl_device_partition_property_ext * /* properties */,
                        ecl_uint /*num_entries*/,
                        ecl_device_id * /*out_devices*/,
                        ecl_uint * /*num_devices*/ ) ECL_EXT_SUFFIX__VERSION_1_1;

typedef ECL_API_ENTRY ecl_int
( ECL_API_CALL * eclCreateSubDevicesEXT_fn)(  ecl_device_id /*in_device*/,
                                            const ecl_device_partition_property_ext * /* properties */,
                                            ecl_uint /*num_entries*/,
                                            ecl_device_id * /*out_devices*/,
                                            ecl_uint * /*num_devices*/ ) ECL_EXT_SUFFIX__VERSION_1_1;

/* ecl_device_partition_property_ext */
#define ECL_DEVICE_PARTITION_EQUALLY_EXT             0x4050
#define ECL_DEVICE_PARTITION_BY_COUNTS_EXT           0x4051
#define ECL_DEVICE_PARTITION_BY_NAMES_EXT            0x4052
#define ECL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN_EXT  0x4053

/* eclDeviceGetInfo selectors */
#define ECL_DEVICE_PARENT_DEVICE_EXT                 0x4054
#define ECL_DEVICE_PARTITION_TYPES_EXT               0x4055
#define ECL_DEVICE_AFFINITY_DOMAINS_EXT              0x4056
#define ECL_DEVICE_REFERENCE_COUNT_EXT               0x4057
#define ECL_DEVICE_PARTITION_STYLE_EXT               0x4058

/* error codes */
#define ECL_DEVICE_PARTITION_FAILED_EXT              -1057
#define ECL_INVALID_PARTITION_COUNT_EXT              -1058
#define ECL_INVALID_PARTITION_NAME_EXT               -1059

/* ECL_AFFINITY_DOMAINs */
#define ECL_AFFINITY_DOMAIN_L1_CACHE_EXT             0x1
#define ECL_AFFINITY_DOMAIN_L2_CACHE_EXT             0x2
#define ECL_AFFINITY_DOMAIN_L3_CACHE_EXT             0x3
#define ECL_AFFINITY_DOMAIN_L4_CACHE_EXT             0x4
#define ECL_AFFINITY_DOMAIN_NUMA_EXT                 0x10
#define ECL_AFFINITY_DOMAIN_NEXT_FISSIONABLE_EXT     0x100

/* ecl_device_partition_property_ext list terminators */
#define ECL_PROPERTIES_LIST_END_EXT                  ((ecl_device_partition_property_ext) 0)
#define ECL_PARTITION_BY_COUNTS_LIST_END_EXT         ((ecl_device_partition_property_ext) 0)
#define ECL_PARTITION_BY_NAMES_LIST_END_EXT          ((ecl_device_partition_property_ext) 0 - 1)


/***********************************
 * ecl_ext_migrate_memobject extension definitions
 ***********************************/
#define ecl_ext_migrate_memobject 1

typedef ecl_bitfield ecl_mem_migration_flags_ext;

#define ECL_MIGRATE_MEM_OBJECT_HOST_EXT              0x1

#define ECL_COMMAND_MIGRATE_MEM_OBJECT_EXT           0x4040

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueMigrateMemObjectEXT( ecl_command_queue /* command_queue */,
                              ecl_uint /* num_mem_objects */,
                              const ecl_mem * /* mem_objects */,
                              ecl_mem_migration_flags_ext /* flags */,
                              ecl_uint /* num_events_in_wait_list */,
                              const ecl_event * /* event_wait_list */,
                              ecl_event * /* event */ );

typedef ECL_API_ENTRY ecl_int
(ECL_API_CALL *eclEnqueueMigrateMemObjectEXT_fn)( ecl_command_queue /* command_queue */,
                                                ecl_uint /* num_mem_objects */,
                                                const ecl_mem * /* mem_objects */,
                                                ecl_mem_migration_flags_ext /* flags */,
                                                ecl_uint /* num_events_in_wait_list */,
                                                const ecl_event * /* event_wait_list */,
                                                ecl_event * /* event */ );


/*********************************
* ecl_qcom_ext_host_ptr extension
*********************************/
#define ecl_qcom_ext_host_ptr 1

#define ECL_MEM_EXT_HOST_PTR_QCOM                  (1 << 29)

#define ECL_DEVICE_EXT_MEM_PADDING_IN_BYTES_QCOM   0x40A0
#define ECL_DEVICE_PAGE_SIZE_QCOM                  0x40A1
#define ECL_IMAGE_ROW_ALIGNMENT_QCOM               0x40A2
#define ECL_IMAGE_SLICE_ALIGNMENT_QCOM             0x40A3
#define ECL_MEM_HOST_UNCACHED_QCOM                 0x40A4
#define ECL_MEM_HOST_WRITEBACK_QCOM                0x40A5
#define ECL_MEM_HOST_WRITETHROUGH_QCOM             0x40A6
#define ECL_MEM_HOST_WRITE_COMBINING_QCOM          0x40A7

typedef ecl_uint                                   ecl_image_pitch_info_qcom;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetDeviceImageInfoQCOM(ecl_device_id             device,
                         size_t                   image_width,
                         size_t                   image_height,
                         const ecl_image_format   *image_format,
                         ecl_image_pitch_info_qcom param_name,
                         size_t                   param_value_size,
                         void                    *param_value,
                         size_t                  *param_value_size_ret);

typedef struct _ecl_mem_ext_host_ptr
{
    /* Type of external memory allocation. */
    /* Legal values will be defined in layered extensions. */
    ecl_uint  allocation_type;

    /* Host cache policy for this external memory allocation. */
    ecl_uint  host_cache_policy;

} ecl_mem_ext_host_ptr;


/*******************************************
* ecl_qcom_ext_host_ptr_iocoherent extension
********************************************/

/* Cache policy specifying io-coherence */
#define ECL_MEM_HOST_IOCOHERENT_QCOM               0x40A9


/*********************************
* ecl_qcom_ion_host_ptr extension
*********************************/

#define ECL_MEM_ION_HOST_PTR_QCOM                  0x40A8

typedef struct _ecl_mem_ion_host_ptr
{
    /* Type of external memory allocation. */
    /* Must be ECL_MEM_ION_HOST_PTR_QCOM for ION allocations. */
    ecl_mem_ext_host_ptr  ext_host_ptr;

    /* ION file descriptor */
    int                  ion_filedesc;

    /* Host pointer to the ION allocated memory */
    void*                ion_hostptr;

} ecl_mem_ion_host_ptr;


/*********************************
* ecl_qcom_android_native_buffer_host_ptr extension
*********************************/

#define ECL_MEM_ANDROID_NATIVE_BUFFER_HOST_PTR_QCOM                  0x40C6

typedef struct _ecl_mem_android_native_buffer_host_ptr
{
    /* Type of external memory allocation. */
    /* Must be ECL_MEM_ANDROID_NATIVE_BUFFER_HOST_PTR_QCOM for Android native buffers. */
    ecl_mem_ext_host_ptr  ext_host_ptr;

    /* Virtual pointer to the android native buffer */
    void*                anb_ptr;

} ecl_mem_android_native_buffer_host_ptr;


/******************************************
 * ecl_img_yuv_image extension *
 ******************************************/

/* Image formats used in eclCreateImage */
#define ECL_NV21_IMG                                 0x40D0
#define ECL_YV12_IMG                                 0x40D1


/******************************************
 * ecl_img_cached_allocations extension *
 ******************************************/

/* Flag values used by eclCreateBuffer */
#define ECL_MEM_USE_UNCACHED_CPU_MEMORY_IMG          (1 << 26)
#define ECL_MEM_USE_CACHED_CPU_MEMORY_IMG            (1 << 27)


/******************************************
 * ecl_img_use_gralloc_ptr extension *
 ******************************************/
#define ecl_img_use_gralloc_ptr 1

/* Flag values used by eclCreateBuffer */
#define ECL_MEM_USE_GRALLOC_PTR_IMG                  (1 << 28)

/* To be used by eclGetEventInfo: */
#define ECL_COMMAND_ACQUIRE_GRALLOC_OBJECTS_IMG      0x40D2
#define ECL_COMMAND_RELEASE_GRALLOC_OBJECTS_IMG      0x40D3

/* Error code from eclEnqueueReleaseGrallocObjectsIMG */
#define ECL_GRALLOC_RESOURCE_NOT_ACQUIRED_IMG        0x40D4

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueAcquireGrallocObjectsIMG(ecl_command_queue      /* command_queue */,
                                  ecl_uint               /* num_objects */,
                                  const ecl_mem *        /* mem_objects */,
                                  ecl_uint               /* num_events_in_wait_list */,
                                  const ecl_event *      /* event_wait_list */,
                                  ecl_event *            /* event */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueReleaseGrallocObjectsIMG(ecl_command_queue      /* command_queue */,
                                  ecl_uint               /* num_objects */,
                                  const ecl_mem *        /* mem_objects */,
                                  ecl_uint               /* num_events_in_wait_list */,
                                  const ecl_event *      /* event_wait_list */,
                                  ecl_event *            /* event */) ECL_EXT_SUFFIX__VERSION_1_2;


/*********************************
* ecl_khr_subgroups extension
*********************************/
#define ecl_khr_subgroups 1

#if !defined(ECL_VERSION_2_1)
/* For OpenCL 2.1 and newer, ecl_kernel_sub_group_info is declared in ECL.h.
   In hindsight, there should have been a khr suffix on this type for
   the extension, but keeping it un-suffixed to maintain backwards
   compatibility. */
typedef ecl_uint             ecl_kernel_sub_group_info;
#endif

/* ecl_kernel_sub_group_info */
#define ECL_KERNEL_MAX_SUB_GROUP_SIZE_FOR_NDRANGE_KHR    0x2033
#define ECL_KERNEL_SUB_GROUP_COUNT_FOR_NDRANGE_KHR       0x2034

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetKernelSubGroupInfoKHR(ecl_kernel /* in_kernel */,
                           ecl_device_id /*in_device*/,
                           ecl_kernel_sub_group_info /* param_name */,
                           size_t /*input_value_size*/,
                           const void * /*input_value*/,
                           size_t /*param_value_size*/,
                           void* /*param_value*/,
                           size_t* /*param_value_size_ret*/ ) ECL_EXT_SUFFIX__VERSION_2_0_DEPRECATED;

typedef ECL_API_ENTRY ecl_int
(ECL_API_CALL * eclGetKernelSubGroupInfoKHR_fn)(ecl_kernel /* in_kernel */,
                              ecl_device_id /*in_device*/,
                              ecl_kernel_sub_group_info /* param_name */,
                              size_t /*input_value_size*/,
                              const void * /*input_value*/,
                              size_t /*param_value_size*/,
                              void* /*param_value*/,
                              size_t* /*param_value_size_ret*/ ) ECL_EXT_SUFFIX__VERSION_2_0_DEPRECATED;


/*********************************
* ecl_khr_mipmap_image extension
*********************************/

/* ecl_sampler_properties */
#define ECL_SAMPLER_MIP_FILTER_MODE_KHR              0x1155
#define ECL_SAMPLER_LOD_MIN_KHR                      0x1156
#define ECL_SAMPLER_LOD_MAX_KHR                      0x1157


/*********************************
* ecl_khr_priority_hints extension
*********************************/
/* This extension define is for backwards compatibility.
   It shouldn't be required since this extension has no new functions. */
#define ecl_khr_priority_hints 1

typedef ecl_uint  ecl_queue_priority_khr;

/* ecl_command_queue_properties */
#define ECL_QUEUE_PRIORITY_KHR 0x1096

/* ecl_queue_priority_khr */
#define ECL_QUEUE_PRIORITY_HIGH_KHR (1<<0)
#define ECL_QUEUE_PRIORITY_MED_KHR (1<<1)
#define ECL_QUEUE_PRIORITY_LOW_KHR (1<<2)


/*********************************
* ecl_khr_throttle_hints extension
*********************************/
/* This extension define is for backwards compatibility.
   It shouldn't be required since this extension has no new functions. */
#define ecl_khr_throttle_hints 1

typedef ecl_uint  ecl_queue_throttle_khr;

/* ecl_command_queue_properties */
#define ECL_QUEUE_THROTTLE_KHR 0x1097

/* ecl_queue_throttle_khr */
#define ECL_QUEUE_THROTTLE_HIGH_KHR (1<<0)
#define ECL_QUEUE_THROTTLE_MED_KHR (1<<1)
#define ECL_QUEUE_THROTTLE_LOW_KHR (1<<2)


/*********************************
* ecl_khr_subgroup_named_barrier
*********************************/
/* This extension define is for backwards compatibility.
   It shouldn't be required since this extension has no new functions. */
#define ecl_khr_subgroup_named_barrier 1

/* ecl_device_info */
#define ECL_DEVICE_MAX_NAMED_BARRIER_COUNT_KHR       0x2035


/**********************************
 * ecl_arm_import_memory extension *
 **********************************/
#define ecl_arm_import_memory 1

typedef intptr_t ecl_import_properties_arm;

/* Default and valid proporties name for ecl_arm_import_memory */
#define ECL_IMPORT_TYPE_ARM                        0x40B2

/* Host process memory type default value for ECL_IMPORT_TYPE_ARM property */
#define ECL_IMPORT_TYPE_HOST_ARM                   0x40B3

/* DMA BUF memory type value for ECL_IMPORT_TYPE_ARM property */
#define ECL_IMPORT_TYPE_DMA_BUF_ARM                0x40B4

/* Protected DMA BUF memory type value for ECL_IMPORT_TYPE_ARM property */
#define ECL_IMPORT_TYPE_PROTECTED_ARM              0x40B5

/* This extension adds a new function that allows for direct memory import into
 * OpenCL via the eclImportMemoryARM function.
 *
 * Memory imported through this interface will be mapped into the device's page
 * tables directly, providing zero copy access. It will never fall back to copy
 * operations and aliased buffers.
 *
 * Types of memory supported for import are specified as additional extension
 * strings.
 *
 * This extension produces ecl_mem allocations which are compatible with all other
 * users of ecl_mem in the standard API.
 *
 * This extension maps pages with the same properties as the normal buffer creation
 * function eclCreateBuffer.
 */
extern ECL_API_ENTRY ecl_mem ECL_API_CALL
eclImportMemoryARM( ecl_context context,
                   ecl_mem_flags flags,
                   const ecl_import_properties_arm *properties,
                   void *memory,
                   size_t size,
                   ecl_int *errcode_ret) ECL_EXT_SUFFIX__VERSION_1_0;


/******************************************
 * ecl_arm_shared_virtual_memory extension *
 ******************************************/
#define ecl_arm_shared_virtual_memory 1

/* Used by eclGetDeviceInfo */
#define ECL_DEVICE_SVM_CAPABILITIES_ARM                  0x40B6

/* Used by eclGetMemObjectInfo */
#define ECL_MEM_USES_SVM_POINTER_ARM                     0x40B7

/* Used by eclSetKernelExecInfoARM: */
#define ECL_KERNEL_EXEC_INFO_SVM_PTRS_ARM                0x40B8
#define ECL_KERNEL_EXEC_INFO_SVM_FINE_GRAIN_SYSTEM_ARM   0x40B9

/* To be used by eclGetEventInfo: */
#define ECL_COMMAND_SVM_FREE_ARM                         0x40BA
#define ECL_COMMAND_SVM_MEMCPY_ARM                       0x40BB
#define ECL_COMMAND_SVM_MEMFILL_ARM                      0x40BC
#define ECL_COMMAND_SVM_MAP_ARM                          0x40BD
#define ECL_COMMAND_SVM_UNMAP_ARM                        0x40BE

/* Flag values returned by eclGetDeviceInfo with ECL_DEVICE_SVM_CAPABILITIES_ARM as the param_name. */
#define ECL_DEVICE_SVM_COARSE_GRAIN_BUFFER_ARM           (1 << 0)
#define ECL_DEVICE_SVM_FINE_GRAIN_BUFFER_ARM             (1 << 1)
#define ECL_DEVICE_SVM_FINE_GRAIN_SYSTEM_ARM             (1 << 2)
#define ECL_DEVICE_SVM_ATOMICS_ARM                       (1 << 3)

/* Flag values used by eclSVMAllocARM: */
#define ECL_MEM_SVM_FINE_GRAIN_BUFFER_ARM                (1 << 10)
#define ECL_MEM_SVM_ATOMICS_ARM                          (1 << 11)

typedef ecl_bitfield ecl_svm_mem_flags_arm;
typedef ecl_uint     ecl_kernel_exec_info_arm;
typedef ecl_bitfield ecl_device_svm_capabilities_arm;

extern ECL_API_ENTRY void * ECL_API_CALL
eclSVMAllocARM(ecl_context       /* context */,
              ecl_svm_mem_flags_arm /* flags */,
              size_t           /* size */,
              ecl_uint          /* alignment */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY void ECL_API_CALL
eclSVMFreeARM(ecl_context        /* context */,
             void *            /* svm_pointer */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueSVMFreeARM(ecl_command_queue  /* command_queue */,
                    ecl_uint           /* num_svm_pointers */,
                    void *[]          /* svm_pointers[] */,
                    void (ECL_CALLBACK * /*pfn_free_func*/)(ecl_command_queue /* queue */,
                                                           ecl_uint          /* num_svm_pointers */,
                                                           void *[]         /* svm_pointers[] */,
                                                           void *           /* user_data */),
                    void *            /* user_data */,
                    ecl_uint           /* num_events_in_wait_list */,
                    const ecl_event *  /* event_wait_list */,
                    ecl_event *        /* event */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueSVMMemcpyARM(ecl_command_queue  /* command_queue */,
                      ecl_bool           /* blocking_copy */,
                      void *            /* dst_ptr */,
                      const void *      /* src_ptr */,
                      size_t            /* size */,
                      ecl_uint           /* num_events_in_wait_list */,
                      const ecl_event *  /* event_wait_list */,
                      ecl_event *        /* event */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueSVMMemFillARM(ecl_command_queue  /* command_queue */,
                       void *            /* svm_ptr */,
                       const void *      /* pattern */,
                       size_t            /* pattern_size */,
                       size_t            /* size */,
                       ecl_uint           /* num_events_in_wait_list */,
                       const ecl_event *  /* event_wait_list */,
                       ecl_event *        /* event */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueSVMMapARM(ecl_command_queue  /* command_queue */,
                   ecl_bool           /* blocking_map */,
                   ecl_map_flags      /* flags */,
                   void *            /* svm_ptr */,
                   size_t            /* size */,
                   ecl_uint           /* num_events_in_wait_list */,
                   const ecl_event *  /* event_wait_list */,
                   ecl_event *        /* event */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueSVMUnmapARM(ecl_command_queue  /* command_queue */,
                     void *            /* svm_ptr */,
                     ecl_uint           /* num_events_in_wait_list */,
                     const ecl_event *  /* event_wait_list */,
                     ecl_event *        /* event */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelArgSVMPointerARM(ecl_kernel    /* kernel */,
                            ecl_uint      /* arg_index */,
                            const void * /* arg_value */) ECL_EXT_SUFFIX__VERSION_1_2;
extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelExecInfoARM(ecl_kernel            /* kernel */,
                       ecl_kernel_exec_info_arm  /* param_name */,
                       size_t               /* param_value_size */,
                       const void *         /* param_value */) ECL_EXT_SUFFIX__VERSION_1_2;

/********************************
 * ecl_arm_get_core_id extension *
 ********************************/

#ifdef ECL_VERSION_1_2

#define ecl_arm_get_core_id 1

/* Device info property for bitfield of cores present */
#define ECL_DEVICE_COMPUTE_UNITS_BITFIELD_ARM      0x40BF

#endif  /* ECL_VERSION_1_2 */

#define ecl_eclore_kernel_arg 1

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelArgELcoreMem(ecl_kernel kernel, ecl_uint arg_index, const ecl_mem arg_value) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelArgELcoreMemNonCached(ecl_kernel kernel, ecl_uint arg_index, const ecl_mem arg_value) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelArgELcoreLocalMem(ecl_kernel kernel, ecl_uint arg_index, const ecl_uint local_mem_size) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelArgDMAMem(ecl_kernel kernel, ecl_uint arg_index, const ecl_mem arg_value) ECL_API_SUFFIX__VERSION_1_0;

#ifdef __cplusplus
}
#endif


#endif /* __ELCORE_ECL_EXT_H */
