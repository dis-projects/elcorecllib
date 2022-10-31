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

#ifndef __ECL_H
#define __ECL_H

#include <elcorecl/ecl_version.h>
#include <elcorecl/ecl_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

typedef struct _ecl_platform_id *    ecl_platform_id;
typedef struct _ecl_device_id *      ecl_device_id;
typedef struct _ecl_context *        ecl_context;
typedef struct _ecl_command_queue *  ecl_command_queue;
typedef struct _ecl_mem *            ecl_mem;
typedef struct _ecl_program *        ecl_program;
typedef struct _ecl_kernel *         ecl_kernel;
typedef struct _ecl_event *          ecl_event;
typedef struct _ecl_sampler *        ecl_sampler;

typedef struct _ecl_shared_section {
    ecl_mem mem;
    ecl_ulong virtual_addr;
} ecl_shared_section;

typedef ecl_uint             ecl_bool;                     /* WARNING!  Unlike ecl_ types in ecl_platform.h, ecl_bool is not guaranteed to be the same size as the bool in kernels. */
typedef ecl_ulong            ecl_bitfield;
typedef ecl_bitfield         ecl_device_type;
typedef ecl_uint             ecl_platform_info;
typedef ecl_uint             ecl_device_info;
typedef ecl_bitfield         ecl_device_fp_config;
typedef ecl_uint             ecl_device_mem_cache_type;
typedef ecl_uint             ecl_device_local_mem_type;
typedef ecl_bitfield         ecl_device_exec_capabilities;
#ifdef ECL_VERSION_2_0
typedef ecl_bitfield         ecl_device_svm_capabilities;
#endif
typedef ecl_bitfield         ecl_command_queue_properties;
#ifdef ECL_VERSION_1_2
typedef intptr_t            ecl_device_partition_property;
typedef ecl_bitfield         ecl_device_affinity_domain;
#endif

typedef intptr_t            ecl_context_properties;
typedef ecl_uint             ecl_context_info;
#ifdef ECL_VERSION_2_0
typedef ecl_bitfield         ecl_queue_properties;
#endif
typedef ecl_uint             ecl_command_queue_info;
typedef ecl_uint             ecl_channel_order;
typedef ecl_uint             ecl_channel_type;
typedef ecl_bitfield         ecl_mem_flags;
#ifdef ECL_VERSION_2_0
typedef ecl_bitfield         ecl_svm_mem_flags;
#endif
typedef ecl_uint             ecl_mem_object_type;
typedef ecl_uint             ecl_mem_info;
#ifdef ECL_VERSION_1_2
typedef ecl_bitfield         ecl_mem_migration_flags;
#endif
typedef ecl_uint             ecl_image_info;
#ifdef ECL_VERSION_1_1
typedef ecl_uint             ecl_buffer_create_type;
#endif
typedef ecl_uint             ecl_addressing_mode;
typedef ecl_uint             ecl_filter_mode;
typedef ecl_uint             ecl_sampler_info;
typedef ecl_bitfield         ecl_map_flags;
#ifdef ECL_VERSION_2_0
typedef intptr_t            ecl_pipe_properties;
typedef ecl_uint             ecl_pipe_info;
#endif
typedef ecl_uint             ecl_program_info;
typedef ecl_uint             ecl_program_build_info;
#ifdef ECL_VERSION_1_2
typedef ecl_uint             ecl_program_binary_type;
#endif
typedef ecl_int              ecl_build_status;
typedef ecl_uint             ecl_kernel_info;
#ifdef ECL_VERSION_1_2
typedef ecl_uint             ecl_kernel_arg_info;
typedef ecl_uint             ecl_kernel_arg_address_qualifier;
typedef ecl_uint             ecl_kernel_arg_access_qualifier;
typedef ecl_bitfield         ecl_kernel_arg_type_qualifier;
#endif
typedef ecl_uint             ecl_kernel_work_group_info;
#ifdef ECL_VERSION_2_1
typedef ecl_uint             ecl_kernel_sub_group_info;
#endif
typedef ecl_uint             ecl_event_info;
typedef ecl_uint             ecl_command_type;
typedef ecl_uint             ecl_profiling_info;
#ifdef ECL_VERSION_2_0
typedef ecl_bitfield         ecl_sampler_properties;
typedef ecl_uint             ecl_kernel_exec_info;
#endif

typedef struct _ecl_image_format {
    ecl_channel_order        image_channel_order;
    ecl_channel_type         image_channel_data_type;
} ecl_image_format;

#ifdef ECL_VERSION_1_2

typedef struct _ecl_image_desc {
    ecl_mem_object_type      image_type;
    size_t                  image_width;
    size_t                  image_height;
    size_t                  image_depth;
    size_t                  image_array_size;
    size_t                  image_row_pitch;
    size_t                  image_slice_pitch;
    ecl_uint                 num_mip_levels;
    ecl_uint                 num_samples;
#ifdef __GNUC__
    __extension__   /* Prevents warnings about anonymous union in -pedantic builds */
#endif
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4201 ) /* Prevents warning about nameless struct/union in /W4 /Za builds */
#endif
    union {
      ecl_mem                  buffer;
      ecl_mem                  mem_object;
    };
#ifdef _MSC_VER
#pragma warning( pop )
#endif
} ecl_image_desc;

#endif

#ifdef ECL_VERSION_1_1

typedef struct _ecl_buffer_region {
    size_t                  origin;
    size_t                  size;
} ecl_buffer_region;

#endif

/******************************************************************************/

/* Error Codes */
#define ECL_SUCCESS                                  0
#define ECL_DEVICE_NOT_FOUND                         -1
#define ECL_DEVICE_NOT_AVAILABLE                     -2
#define ECL_COMPILER_NOT_AVAILABLE                   -3
#define ECL_MEM_OBJECT_ALLOCATION_FAILURE            -4
#define ECL_OUT_OF_RESOURCES                         -5
#define ECL_OUT_OF_HOST_MEMORY                       -6
#define ECL_PROFILING_INFO_NOT_AVAILABLE             -7
#define ECL_MEM_COPY_OVERLAP                         -8
#define ECL_IMAGE_FORMAT_MISMATCH                    -9
#define ECL_IMAGE_FORMAT_NOT_SUPPORTED               -10
#define ECL_BUILD_PROGRAM_FAILURE                    -11
#define ECL_MAP_FAILURE                              -12
#ifdef ECL_VERSION_1_1
#define ECL_MISALIGNED_SUB_BUFFER_OFFSET             -13
#define ECL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST -14
#endif
#ifdef ECL_VERSION_1_2
#define ECL_COMPILE_PROGRAM_FAILURE                  -15
#define ECL_LINKER_NOT_AVAILABLE                     -16
#define ECL_LINK_PROGRAM_FAILURE                     -17
#define ECL_DEVICE_PARTITION_FAILED                  -18
#define ECL_KERNEL_ARG_INFO_NOT_AVAILABLE            -19
#endif

#define ECL_INVALID_VALUE                            -30
#define ECL_INVALID_DEVICE_TYPE                      -31
#define ECL_INVALID_PLATFORM                         -32
#define ECL_INVALID_DEVICE                           -33
#define ECL_INVALID_CONTEXT                          -34
#define ECL_INVALID_QUEUE_PROPERTIES                 -35
#define ECL_INVALID_COMMAND_QUEUE                    -36
#define ECL_INVALID_HOST_PTR                         -37
#define ECL_INVALID_MEM_OBJECT                       -38
#define ECL_INVALID_IMAGE_FORMAT_DESCRIPTOR          -39
#define ECL_INVALID_IMAGE_SIZE                       -40
#define ECL_INVALID_SAMPLER                          -41
#define ECL_INVALID_BINARY                           -42
#define ECL_INVALID_BUILD_OPTIONS                    -43
#define ECL_INVALID_PROGRAM                          -44
#define ECL_INVALID_PROGRAM_EXECUTABLE               -45
#define ECL_INVALID_KERNEL_NAME                      -46
#define ECL_INVALID_KERNEL_DEFINITION                -47
#define ECL_INVALID_KERNEL                           -48
#define ECL_INVALID_ARG_INDEX                        -49
#define ECL_INVALID_ARG_VALUE                        -50
#define ECL_INVALID_ARG_SIZE                         -51
#define ECL_INVALID_KERNEL_ARGS                      -52
#define ECL_INVALID_WORK_DIMENSION                   -53
#define ECL_INVALID_WORK_GROUP_SIZE                  -54
#define ECL_INVALID_WORK_ITEM_SIZE                   -55
#define ECL_INVALID_GLOBAL_OFFSET                    -56
#define ECL_INVALID_EVENT_WAIT_LIST                  -57
#define ECL_INVALID_EVENT                            -58
#define ECL_INVALID_OPERATION                        -59
#define ECL_INVALID_GL_OBJECT                        -60
#define ECL_INVALID_BUFFER_SIZE                      -61
#define ECL_INVALID_MIP_LEVEL                        -62
#define ECL_INVALID_GLOBAL_WORK_SIZE                 -63
#ifdef ECL_VERSION_1_1
#define ECL_INVALID_PROPERTY                         -64
#endif
#ifdef ECL_VERSION_1_2
#define ECL_INVALID_IMAGE_DESCRIPTOR                 -65
#define ECL_INVALID_COMPILER_OPTIONS                 -66
#define ECL_INVALID_LINKER_OPTIONS                   -67
#define ECL_INVALID_DEVICE_PARTITION_COUNT           -68
#endif
#ifdef ECL_VERSION_2_0
#define ECL_INVALID_PIPE_SIZE                        -69
#define ECL_INVALID_DEVICE_QUEUE                     -70
#endif
#ifdef ECL_VERSION_2_2
#define ECL_INVALID_SPEC_ID                          -71
#define ECL_MAX_SIZE_RESTRICTION_EXCEEDED            -72
#endif


/* ecl_bool */
#define ECL_FALSE                                    0
#define ECL_TRUE                                     1
#ifdef ECL_VERSION_1_2
#define ECL_BLOCKING                                 ECL_TRUE
#define ECL_NON_BLOCKING                             ECL_FALSE
#endif

/* ecl_platform_info */
#define ECL_PLATFORM_PROFILE                         0x0900
#define ECL_PLATFORM_VERSION                         0x0901
#define ECL_PLATFORM_NAME                            0x0902
#define ECL_PLATFORM_VENDOR                          0x0903
#define ECL_PLATFORM_EXTENSIONS                      0x0904
#ifdef ECL_VERSION_2_1
#define ECL_PLATFORM_HOST_TIMER_RESOLUTION           0x0905
#endif

/* ecl_device_type - bitfield */
#define ECL_DEVICE_TYPE_DEFAULT                      (1 << 0)
#define ECL_DEVICE_TYPE_CPU                          (1 << 1)
#define ECL_DEVICE_TYPE_GPU                          (1 << 2)
#define ECL_DEVICE_TYPE_ACCELERATOR                  (1 << 3)
#ifdef ECL_VERSION_1_2
#define ECL_DEVICE_TYPE_CUSTOM                       (1 << 4)
#endif
#define ECL_DEVICE_TYPE_ALL                          0xFFFFFFFF

/* ecl_device_info */
#define ECL_DEVICE_TYPE                                   0x1000
#define ECL_DEVICE_VENDOR_ID                              0x1001
#define ECL_DEVICE_MAX_COMPUTE_UNITS                      0x1002
#define ECL_DEVICE_MAX_WORK_ITEM_DIMENSIONS               0x1003
#define ECL_DEVICE_MAX_WORK_GROUP_SIZE                    0x1004
#define ECL_DEVICE_MAX_WORK_ITEM_SIZES                    0x1005
#define ECL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR            0x1006
#define ECL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT           0x1007
#define ECL_DEVICE_PREFERRED_VECTOR_WIDTH_INT             0x1008
#define ECL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG            0x1009
#define ECL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT           0x100A
#define ECL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE          0x100B
#define ECL_DEVICE_MAX_CLOCK_FREQUENCY                    0x100C
#define ECL_DEVICE_ADDRESS_BITS                           0x100D
#define ECL_DEVICE_MAX_READ_IMAGE_ARGS                    0x100E
#define ECL_DEVICE_MAX_WRITE_IMAGE_ARGS                   0x100F
#define ECL_DEVICE_MAX_MEM_ALLOC_SIZE                     0x1010
#define ECL_DEVICE_IMAGE2D_MAX_WIDTH                      0x1011
#define ECL_DEVICE_IMAGE2D_MAX_HEIGHT                     0x1012
#define ECL_DEVICE_IMAGE3D_MAX_WIDTH                      0x1013
#define ECL_DEVICE_IMAGE3D_MAX_HEIGHT                     0x1014
#define ECL_DEVICE_IMAGE3D_MAX_DEPTH                      0x1015
#define ECL_DEVICE_IMAGE_SUPPORT                          0x1016
#define ECL_DEVICE_MAX_PARAMETER_SIZE                     0x1017
#define ECL_DEVICE_MAX_SAMPLERS                           0x1018
#define ECL_DEVICE_MEM_BASE_ADDR_ALIGN                    0x1019
#define ECL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE               0x101A
#define ECL_DEVICE_SINGLE_FP_CONFIG                       0x101B
#define ECL_DEVICE_GLOBAL_MEM_CACHE_TYPE                  0x101C
#define ECL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE              0x101D
#define ECL_DEVICE_GLOBAL_MEM_CACHE_SIZE                  0x101E
#define ECL_DEVICE_GLOBAL_MEM_SIZE                        0x101F
#define ECL_DEVICE_MAX_CONSTANT_BUFFER_SIZE               0x1020
#define ECL_DEVICE_MAX_CONSTANT_ARGS                      0x1021
#define ECL_DEVICE_LOCAL_MEM_TYPE                         0x1022
#define ECL_DEVICE_LOCAL_MEM_SIZE                         0x1023
#define ECL_DEVICE_ERROR_CORRECTION_SUPPORT               0x1024
#define ECL_DEVICE_PROFILING_TIMER_RESOLUTION             0x1025
#define ECL_DEVICE_ENDIAN_LITTLE                          0x1026
#define ECL_DEVICE_AVAILABLE                              0x1027
#define ECL_DEVICE_COMPILER_AVAILABLE                     0x1028
#define ECL_DEVICE_EXECUTION_CAPABILITIES                 0x1029
#define ECL_DEVICE_QUEUE_PROPERTIES                       0x102A    /* deprecated */
#ifdef ECL_VERSION_2_0
#define ECL_DEVICE_QUEUE_ON_HOST_PROPERTIES               0x102A
#endif
#define ECL_DEVICE_NAME                                   0x102B
#define ECL_DEVICE_VENDOR                                 0x102C
#define ECL_DRIVER_VERSION                                0x102D
#define ECL_DEVICE_PROFILE                                0x102E
#define ECL_DEVICE_VERSION                                0x102F
#define ECL_DEVICE_EXTENSIONS                             0x1030
#define ECL_DEVICE_PLATFORM                               0x1031
#ifdef ECL_VERSION_1_2
#define ECL_DEVICE_DOUBLE_FP_CONFIG                       0x1032
#endif
/* 0x1033 reserved for ECL_DEVICE_HALF_FP_CONFIG which is already defined in "ecl_ext.h" */
#ifdef ECL_VERSION_1_1
#define ECL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF            0x1034
#define ECL_DEVICE_HOST_UNIFIED_MEMORY                    0x1035   /* deprecated */
#define ECL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR               0x1036
#define ECL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT              0x1037
#define ECL_DEVICE_NATIVE_VECTOR_WIDTH_INT                0x1038
#define ECL_DEVICE_NATIVE_VECTOR_WIDTH_LONG               0x1039
#define ECL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT              0x103A
#define ECL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE             0x103B
#define ECL_DEVICE_NATIVE_VECTOR_WIDTH_HALF               0x103C
#define ECL_DEVICE_OPENCL_C_VERSION                       0x103D
#endif
#ifdef ECL_VERSION_1_2
#define ECL_DEVICE_LINKER_AVAILABLE                       0x103E
#define ECL_DEVICE_BUILT_IN_KERNELS                       0x103F
#define ECL_DEVICE_IMAGE_MAX_BUFFER_SIZE                  0x1040
#define ECL_DEVICE_IMAGE_MAX_ARRAY_SIZE                   0x1041
#define ECL_DEVICE_PARENT_DEVICE                          0x1042
#define ECL_DEVICE_PARTITION_MAX_SUB_DEVICES              0x1043
#define ECL_DEVICE_PARTITION_PROPERTIES                   0x1044
#define ECL_DEVICE_PARTITION_AFFINITY_DOMAIN              0x1045
#define ECL_DEVICE_PARTITION_TYPE                         0x1046
#define ECL_DEVICE_REFERENCE_COUNT                        0x1047
#define ECL_DEVICE_PREFERRED_INTEROP_USER_SYNC            0x1048
#define ECL_DEVICE_PRINTF_BUFFER_SIZE                     0x1049
#define ECL_DEVICE_IMAGE_PITCH_ALIGNMENT                  0x104A
#define ECL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT           0x104B
#endif
#ifdef ECL_VERSION_2_0
#define ECL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS              0x104C
#define ECL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE               0x104D
#define ECL_DEVICE_QUEUE_ON_DEVICE_PROPERTIES             0x104E
#define ECL_DEVICE_QUEUE_ON_DEVICE_PREFERRED_SIZE         0x104F
#define ECL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE               0x1050
#define ECL_DEVICE_MAX_ON_DEVICE_QUEUES                   0x1051
#define ECL_DEVICE_MAX_ON_DEVICE_EVENTS                   0x1052
#define ECL_DEVICE_SVM_CAPABILITIES                       0x1053
#define ECL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE   0x1054
#define ECL_DEVICE_MAX_PIPE_ARGS                          0x1055
#define ECL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS           0x1056
#define ECL_DEVICE_PIPE_MAX_PACKET_SIZE                   0x1057
#define ECL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT    0x1058
#define ECL_DEVICE_PREFERRED_GLOBAL_ATOMIC_ALIGNMENT      0x1059
#define ECL_DEVICE_PREFERRED_LOCAL_ATOMIC_ALIGNMENT       0x105A
#endif
#ifdef ECL_VERSION_2_1
#define ECL_DEVICE_IL_VERSION                             0x105B
#define ECL_DEVICE_MAX_NUM_SUB_GROUPS                     0x105C
#define ECL_DEVICE_SUB_GROUP_INDEPENDENT_FORWARD_PROGRESS 0x105D
#endif
#define ECL_DEVICE_MAX_DMAMEM_ALLOC_SIZE                  0x1285

/* ecl_device_fp_config - bitfield */
#define ECL_FP_DENORM                                (1 << 0)
#define ECL_FP_INF_NAN                               (1 << 1)
#define ECL_FP_ROUND_TO_NEAREST                      (1 << 2)
#define ECL_FP_ROUND_TO_ZERO                         (1 << 3)
#define ECL_FP_ROUND_TO_INF                          (1 << 4)
#define ECL_FP_FMA                                   (1 << 5)
#ifdef ECL_VERSION_1_1
#define ECL_FP_SOFT_FLOAT                            (1 << 6)
#endif
#ifdef ECL_VERSION_1_2
#define ECL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT         (1 << 7)
#endif

/* ecl_device_mem_cache_type */
#define ECL_NONE                                     0x0
#define ECL_READ_ONLY_CACHE                          0x1
#define ECL_READ_WRITE_CACHE                         0x2

/* ecl_device_local_mem_type */
#define ECL_LOCAL                                    0x1
#define ECL_GLOBAL                                   0x2

/* ecl_device_exec_capabilities - bitfield */
#define ECL_EXEC_KERNEL                              (1 << 0)
#define ECL_EXEC_NATIVE_KERNEL                       (1 << 1)

/* ecl_command_queue_properties - bitfield */
#define ECL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE      (1 << 0)
#define ECL_QUEUE_PROFILING_ENABLE                   (1 << 1)
#ifdef ECL_VERSION_2_0
#define ECL_QUEUE_ON_DEVICE                          (1 << 2)
#define ECL_QUEUE_ON_DEVICE_DEFAULT                  (1 << 3)
#endif

/* ecl_context_info  */
#define ECL_CONTEXT_REFERENCE_COUNT                  0x1080
#define ECL_CONTEXT_DEVICES                          0x1081
#define ECL_CONTEXT_PROPERTIES                       0x1082
#ifdef ECL_VERSION_1_1
#define ECL_CONTEXT_NUM_DEVICES                      0x1083
#endif

/* ecl_context_properties */
#define ECL_CONTEXT_PLATFORM                         0x1084
#ifdef ECL_VERSION_1_2
#define ECL_CONTEXT_INTEROP_USER_SYNC                0x1085
#endif

#ifdef ECL_VERSION_1_2

/* ecl_device_partition_property */
#define ECL_DEVICE_PARTITION_EQUALLY                 0x1086
#define ECL_DEVICE_PARTITION_BY_COUNTS               0x1087
#define ECL_DEVICE_PARTITION_BY_COUNTS_LIST_END      0x0
#define ECL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN      0x1088

#endif

#ifdef ECL_VERSION_1_2

/* ecl_device_affinity_domain */
#define ECL_DEVICE_AFFINITY_DOMAIN_NUMA               (1 << 0)
#define ECL_DEVICE_AFFINITY_DOMAIN_L4_CACHE           (1 << 1)
#define ECL_DEVICE_AFFINITY_DOMAIN_L3_CACHE           (1 << 2)
#define ECL_DEVICE_AFFINITY_DOMAIN_L2_CACHE           (1 << 3)
#define ECL_DEVICE_AFFINITY_DOMAIN_L1_CACHE           (1 << 4)
#define ECL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE (1 << 5)

#endif

#ifdef ECL_VERSION_2_0

/* ecl_device_svm_capabilities */
#define ECL_DEVICE_SVM_COARSE_GRAIN_BUFFER           (1 << 0)
#define ECL_DEVICE_SVM_FINE_GRAIN_BUFFER             (1 << 1)
#define ECL_DEVICE_SVM_FINE_GRAIN_SYSTEM             (1 << 2)
#define ECL_DEVICE_SVM_ATOMICS                       (1 << 3)

#endif

/* ecl_command_queue_info */
#define ECL_QUEUE_CONTEXT                            0x1090
#define ECL_QUEUE_DEVICE                             0x1091
#define ECL_QUEUE_REFERENCE_COUNT                    0x1092
#define ECL_QUEUE_PROPERTIES                         0x1093
#ifdef ECL_VERSION_2_0
#define ECL_QUEUE_SIZE                               0x1094
#endif
#ifdef ECL_VERSION_2_1
#define ECL_QUEUE_DEVICE_DEFAULT                     0x1095
#endif

/* ecl_mem_flags and ecl_svm_mem_flags - bitfield */
#define ECL_MEM_READ_WRITE                           (1 << 0)
#define ECL_MEM_WRITE_ONLY                           (1 << 1)
#define ECL_MEM_READ_ONLY                            (1 << 2)
#define ECL_MEM_USE_HOST_PTR                         (1 << 3)
#define ECL_MEM_ALLOC_HOST_PTR                       (1 << 4)
#define ECL_MEM_COPY_HOST_PTR                        (1 << 5)
#define ECL_MEM_USE_DMABUF                           (1 << 6)
#ifdef ECL_VERSION_1_2
#define ECL_MEM_HOST_WRITE_ONLY                      (1 << 7)
#define ECL_MEM_HOST_READ_ONLY                       (1 << 8)
#define ECL_MEM_HOST_NO_ACCESS                       (1 << 9)
#endif
#ifdef ECL_VERSION_2_0
#define ECL_MEM_SVM_FINE_GRAIN_BUFFER                (1 << 10)   /* used by ecl_svm_mem_flags only */
#define ECL_MEM_SVM_ATOMICS                          (1 << 11)   /* used by ecl_svm_mem_flags only */
#define ECL_MEM_KERNEL_READ_AND_WRITE                (1 << 12)
#endif

#ifdef ECL_VERSION_1_2

/* ecl_mem_migration_flags - bitfield */
#define ECL_MIGRATE_MEM_OBJECT_HOST                  (1 << 0)
#define ECL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED     (1 << 1)

#endif

/* ecl_channel_order */
#define ECL_R                                        0x10B0
#define ECL_A                                        0x10B1
#define ECL_RG                                       0x10B2
#define ECL_RA                                       0x10B3
#define ECL_RGB                                      0x10B4
#define ECL_RGBA                                     0x10B5
#define ECL_BGRA                                     0x10B6
#define ECL_ARGB                                     0x10B7
#define ECL_INTENSITY                                0x10B8
#define ECL_LUMINANCE                                0x10B9
#ifdef ECL_VERSION_1_1
#define ECL_Rx                                       0x10BA
#define ECL_RGx                                      0x10BB
#define ECL_RGBx                                     0x10BC
#endif
#ifdef ECL_VERSION_1_2
#define ECL_DEPTH                                    0x10BD
#define ECL_DEPTH_STENCIL                            0x10BE
#endif
#ifdef ECL_VERSION_2_0
#define ECL_sRGB                                     0x10BF
#define ECL_sRGBx                                    0x10C0
#define ECL_sRGBA                                    0x10C1
#define ECL_sBGRA                                    0x10C2
#define ECL_ABGR                                     0x10C3
#endif

/* ecl_channel_type */
#define ECL_SNORM_INT8                               0x10D0
#define ECL_SNORM_INT16                              0x10D1
#define ECL_UNORM_INT8                               0x10D2
#define ECL_UNORM_INT16                              0x10D3
#define ECL_UNORM_SHORT_565                          0x10D4
#define ECL_UNORM_SHORT_555                          0x10D5
#define ECL_UNORM_INT_101010                         0x10D6
#define ECL_SIGNED_INT8                              0x10D7
#define ECL_SIGNED_INT16                             0x10D8
#define ECL_SIGNED_INT32                             0x10D9
#define ECL_UNSIGNED_INT8                            0x10DA
#define ECL_UNSIGNED_INT16                           0x10DB
#define ECL_UNSIGNED_INT32                           0x10DC
#define ECL_HALF_FLOAT                               0x10DD
#define ECL_FLOAT                                    0x10DE
#ifdef ECL_VERSION_1_2
#define ECL_UNORM_INT24                              0x10DF
#endif
#ifdef ECL_VERSION_2_1
#define ECL_UNORM_INT_101010_2                       0x10E0
#endif

/* ecl_mem_object_type */
#define ECL_MEM_OBJECT_BUFFER                        0x10F0
#define ECL_MEM_OBJECT_IMAGE2D                       0x10F1
#define ECL_MEM_OBJECT_IMAGE3D                       0x10F2
#ifdef ECL_VERSION_1_2
#define ECL_MEM_OBJECT_IMAGE2D_ARRAY                 0x10F3
#define ECL_MEM_OBJECT_IMAGE1D                       0x10F4
#define ECL_MEM_OBJECT_IMAGE1D_ARRAY                 0x10F5
#define ECL_MEM_OBJECT_IMAGE1D_BUFFER                0x10F6
#endif
#ifdef ECL_VERSION_2_0
#define ECL_MEM_OBJECT_PIPE                          0x10F7
#endif

/* ecl_mem_info */
#define ECL_MEM_TYPE                                 0x1100
#define ECL_MEM_FLAGS                                0x1101
#define ECL_MEM_SIZE                                 0x1102
#define ECL_MEM_HOST_PTR                             0x1103
#define ECL_MEM_MAP_COUNT                            0x1104
#define ECL_MEM_REFERENCE_COUNT                      0x1105
#define ECL_MEM_CONTEXT                              0x1106
#ifdef ECL_VERSION_1_1
#define ECL_MEM_ASSOCIATED_MEMOBJECT                 0x1107
#define ECL_MEM_OFFSET                               0x1108
#endif
#ifdef ECL_VERSION_2_0
#define ECL_MEM_USES_SVM_POINTER                     0x1109
#endif

/* ecl_image_info */
#define ECL_IMAGE_FORMAT                             0x1110
#define ECL_IMAGE_ELEMENT_SIZE                       0x1111
#define ECL_IMAGE_ROW_PITCH                          0x1112
#define ECL_IMAGE_SLICE_PITCH                        0x1113
#define ECL_IMAGE_WIDTH                              0x1114
#define ECL_IMAGE_HEIGHT                             0x1115
#define ECL_IMAGE_DEPTH                              0x1116
#ifdef ECL_VERSION_1_2
#define ECL_IMAGE_ARRAY_SIZE                         0x1117
#define ECL_IMAGE_BUFFER                             0x1118
#define ECL_IMAGE_NUM_MIP_LEVELS                     0x1119
#define ECL_IMAGE_NUM_SAMPLES                        0x111A
#endif

#ifdef ECL_VERSION_2_0

/* ecl_pipe_info */
#define ECL_PIPE_PACKET_SIZE                         0x1120
#define ECL_PIPE_MAX_PACKETS                         0x1121

#endif

/* ecl_addressing_mode */
#define ECL_ADDRESS_NONE                             0x1130
#define ECL_ADDRESS_CLAMP_TO_EDGE                    0x1131
#define ECL_ADDRESS_CLAMP                            0x1132
#define ECL_ADDRESS_REPEAT                           0x1133
#ifdef ECL_VERSION_1_1
#define ECL_ADDRESS_MIRRORED_REPEAT                  0x1134
#endif

/* ecl_filter_mode */
#define ECL_FILTER_NEAREST                           0x1140
#define ECL_FILTER_LINEAR                            0x1141

/* ecl_sampler_info */
#define ECL_SAMPLER_REFERENCE_COUNT                  0x1150
#define ECL_SAMPLER_CONTEXT                          0x1151
#define ECL_SAMPLER_NORMALIZED_COORDS                0x1152
#define ECL_SAMPLER_ADDRESSING_MODE                  0x1153
#define ECL_SAMPLER_FILTER_MODE                      0x1154
#ifdef ECL_VERSION_2_0
/* These enumerants are for the ecl_khr_mipmap_image extension.
   They have since been added to ecl_ext.h with an appropriate
   KHR suffix, but are left here for backwards compatibility. */
#define ECL_SAMPLER_MIP_FILTER_MODE                  0x1155
#define ECL_SAMPLER_LOD_MIN                          0x1156
#define ECL_SAMPLER_LOD_MAX                          0x1157
#endif

/* ecl_map_flags - bitfield */
#define ECL_MAP_READ                                 (1 << 0)
#define ECL_MAP_WRITE                                (1 << 1)
#ifdef ECL_VERSION_1_2
#define ECL_MAP_WRITE_INVALIDATE_REGION              (1 << 2)
#endif

/* ecl_program_info */
#define ECL_PROGRAM_REFERENCE_COUNT                  0x1160
#define ECL_PROGRAM_CONTEXT                          0x1161
#define ECL_PROGRAM_NUM_DEVICES                      0x1162
#define ECL_PROGRAM_DEVICES                          0x1163
#define ECL_PROGRAM_SOURCE                           0x1164
#define ECL_PROGRAM_BINARY_SIZES                     0x1165
#define ECL_PROGRAM_BINARIES                         0x1166
#ifdef ECL_VERSION_1_2
#define ECL_PROGRAM_NUM_KERNELS                      0x1167
#define ECL_PROGRAM_KERNEL_NAMES                     0x1168
#endif
#ifdef ECL_VERSION_2_1
#define ECL_PROGRAM_IL                               0x1169
#endif
#ifdef ECL_VERSION_2_2
#define ECL_PROGRAM_SCOPE_GLOBAL_CTORS_PRESENT       0x116A
#define ECL_PROGRAM_SCOPE_GLOBAL_DTORS_PRESENT       0x116B
#endif
#define ECL_PROGRAM_STACK_SIZE                       0x1286

/* ecl_program_build_info */
#define ECL_PROGRAM_BUILD_STATUS                     0x1181
#define ECL_PROGRAM_BUILD_OPTIONS                    0x1182
#define ECL_PROGRAM_BUILD_LOG                        0x1183
#ifdef ECL_VERSION_1_2
#define ECL_PROGRAM_BINARY_TYPE                      0x1184
#endif
#ifdef ECL_VERSION_2_0
#define ECL_PROGRAM_BUILD_GLOBAL_VARIABLE_TOTAL_SIZE 0x1185
#endif

#ifdef ECL_VERSION_1_2

/* ecl_program_binary_type */
#define ECL_PROGRAM_BINARY_TYPE_NONE                 0x0
#define ECL_PROGRAM_BINARY_TYPE_COMPILED_OBJECT      0x1
#define ECL_PROGRAM_BINARY_TYPE_LIBRARY              0x2
#define ECL_PROGRAM_BINARY_TYPE_EXECUTABLE           0x4

#endif

/* ecl_build_status */
#define ECL_BUILD_SUCCESS                            0
#define ECL_BUILD_NONE                               -1
#define ECL_BUILD_ERROR                              -2
#define ECL_BUILD_IN_PROGRESS                        -3

/* ecl_kernel_info */
#define ECL_KERNEL_FUNCTION_NAME                     0x1190
#define ECL_KERNEL_NUM_ARGS                          0x1191
#define ECL_KERNEL_REFERENCE_COUNT                   0x1192
#define ECL_KERNEL_CONTEXT                           0x1193
#define ECL_KERNEL_PROGRAM                           0x1194
#ifdef ECL_VERSION_1_2
#define ECL_KERNEL_ATTRIBUTES                        0x1195
#endif
#ifdef ECL_VERSION_2_1
#define ECL_KERNEL_MAX_NUM_SUB_GROUPS                0x11B9
#define ECL_KERNEL_COMPILE_NUM_SUB_GROUPS            0x11BA
#endif

#ifdef ECL_VERSION_1_2

/* ecl_kernel_arg_info */
#define ECL_KERNEL_ARG_ADDRESS_QUALIFIER             0x1196
#define ECL_KERNEL_ARG_ACCESS_QUALIFIER              0x1197
#define ECL_KERNEL_ARG_TYPE_NAME                     0x1198
#define ECL_KERNEL_ARG_TYPE_QUALIFIER                0x1199
#define ECL_KERNEL_ARG_NAME                          0x119A

#endif

#ifdef ECL_VERSION_1_2

/* ecl_kernel_arg_address_qualifier */
#define ECL_KERNEL_ARG_ADDRESS_GLOBAL                0x119B
#define ECL_KERNEL_ARG_ADDRESS_LOCAL                 0x119C
#define ECL_KERNEL_ARG_ADDRESS_CONSTANT              0x119D
#define ECL_KERNEL_ARG_ADDRESS_PRIVATE               0x119E

#endif

#ifdef ECL_VERSION_1_2

/* ecl_kernel_arg_access_qualifier */
#define ECL_KERNEL_ARG_ACCESS_READ_ONLY              0x11A0
#define ECL_KERNEL_ARG_ACCESS_WRITE_ONLY             0x11A1
#define ECL_KERNEL_ARG_ACCESS_READ_WRITE             0x11A2
#define ECL_KERNEL_ARG_ACCESS_NONE                   0x11A3

#endif

#ifdef ECL_VERSION_1_2

/* ecl_kernel_arg_type_qualifier */
#define ECL_KERNEL_ARG_TYPE_NONE                     0
#define ECL_KERNEL_ARG_TYPE_CONST                    (1 << 0)
#define ECL_KERNEL_ARG_TYPE_RESTRICT                 (1 << 1)
#define ECL_KERNEL_ARG_TYPE_VOLATILE                 (1 << 2)
#ifdef ECL_VERSION_2_0
#define ECL_KERNEL_ARG_TYPE_PIPE                     (1 << 3)
#endif

#endif

/* ecl_kernel_work_group_info */
#define ECL_KERNEL_WORK_GROUP_SIZE                   0x11B0
#define ECL_KERNEL_COMPILE_WORK_GROUP_SIZE           0x11B1
#define ECL_KERNEL_LOCAL_MEM_SIZE                    0x11B2
#define ECL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE 0x11B3
#define ECL_KERNEL_PRIVATE_MEM_SIZE                  0x11B4
#ifdef ECL_VERSION_1_2
#define ECL_KERNEL_GLOBAL_WORK_SIZE                  0x11B5
#endif

#ifdef ECL_VERSION_2_1

/* ecl_kernel_sub_group_info */
#define ECL_KERNEL_MAX_SUB_GROUP_SIZE_FOR_NDRANGE    0x2033
#define ECL_KERNEL_SUB_GROUP_COUNT_FOR_NDRANGE       0x2034
#define ECL_KERNEL_LOCAL_SIZE_FOR_SUB_GROUP_COUNT    0x11B8

#endif

#ifdef ECL_VERSION_2_0

/* ecl_kernel_exec_info */
#define ECL_KERNEL_EXEC_INFO_SVM_PTRS                0x11B6
#define ECL_KERNEL_EXEC_INFO_SVM_FINE_GRAIN_SYSTEM   0x11B7

#endif

/* ecl_event_info  */
#define ECL_EVENT_COMMAND_QUEUE                      0x11D0
#define ECL_EVENT_COMMAND_TYPE                       0x11D1
#define ECL_EVENT_REFERENCE_COUNT                    0x11D2
#define ECL_EVENT_COMMAND_EXECUTION_STATUS           0x11D3
#ifdef ECL_VERSION_1_1
#define ECL_EVENT_CONTEXT                            0x11D4
#endif

/* ecl_command_type */
#define ECL_COMMAND_NDRANGE_KERNEL                   0x11F0
#define ECL_COMMAND_TASK                             0x11F1
#define ECL_COMMAND_NATIVE_KERNEL                    0x11F2
#define ECL_COMMAND_READ_BUFFER                      0x11F3
#define ECL_COMMAND_WRITE_BUFFER                     0x11F4
#define ECL_COMMAND_COPY_BUFFER                      0x11F5
#define ECL_COMMAND_READ_IMAGE                       0x11F6
#define ECL_COMMAND_WRITE_IMAGE                      0x11F7
#define ECL_COMMAND_COPY_IMAGE                       0x11F8
#define ECL_COMMAND_COPY_IMAGE_TO_BUFFER             0x11F9
#define ECL_COMMAND_COPY_BUFFER_TO_IMAGE             0x11FA
#define ECL_COMMAND_MAP_BUFFER                       0x11FB
#define ECL_COMMAND_MAP_IMAGE                        0x11FC
#define ECL_COMMAND_UNMAP_MEM_OBJECT                 0x11FD
#define ECL_COMMAND_MARKER                           0x11FE
#define ECL_COMMAND_ACQUIRE_GL_OBJECTS               0x11FF
#define ECL_COMMAND_RELEASE_GL_OBJECTS               0x1200
#ifdef ECL_VERSION_1_1
#define ECL_COMMAND_READ_BUFFER_RECT                 0x1201
#define ECL_COMMAND_WRITE_BUFFER_RECT                0x1202
#define ECL_COMMAND_COPY_BUFFER_RECT                 0x1203
#define ECL_COMMAND_USER                             0x1204
#endif
#ifdef ECL_VERSION_1_2
#define ECL_COMMAND_BARRIER                          0x1205
#define ECL_COMMAND_MIGRATE_MEM_OBJECTS              0x1206
#define ECL_COMMAND_FILL_BUFFER                      0x1207
#define ECL_COMMAND_FILL_IMAGE                       0x1208
#endif
#ifdef ECL_VERSION_2_0
#define ECL_COMMAND_SVM_FREE                         0x1209
#define ECL_COMMAND_SVM_MEMCPY                       0x120A
#define ECL_COMMAND_SVM_MEMFILL                      0x120B
#define ECL_COMMAND_SVM_MAP                          0x120C
#define ECL_COMMAND_SVM_UNMAP                        0x120D
#endif

/* command execution status */
#define ECL_COMPLETE                                 0x0
#define ECL_RUNNING                                  0x1
#define ECL_SUBMITTED                                0x2
#define ECL_QUEUED                                   0x3

#ifdef ECL_VERSION_1_1

/* ecl_buffer_create_type  */
#define ECL_BUFFER_CREATE_TYPE_REGION                0x1220

#endif

/* ecl_profiling_info  */
#define ECL_PROFILING_COMMAND_QUEUED                 0x1280
#define ECL_PROFILING_COMMAND_SUBMIT                 0x1281
#define ECL_PROFILING_COMMAND_START                  0x1282
#define ECL_PROFILING_COMMAND_END                    0x1283
#ifdef ECL_VERSION_2_0
#define ECL_PROFILING_COMMAND_COMPLETE               0x1284
#endif

/********************************************************************************************************/

/* Platform API */
extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetPlatformIDs(ecl_uint          /* num_entries */,
                 ecl_platform_id * /* platforms */,
                 ecl_uint *        /* num_platforms */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetPlatformInfo(ecl_platform_id   /* platform */,
                  ecl_platform_info /* param_name */,
                  size_t           /* param_value_size */,
                  void *           /* param_value */,
                  size_t *         /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

/* Device APIs */
extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetDeviceIDs(ecl_platform_id   /* platform */,
               ecl_device_type   /* device_type */,
               ecl_uint          /* num_entries */,
               ecl_device_id *   /* devices */,
               ecl_uint *        /* num_devices */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetDeviceInfo(ecl_device_id    /* device */,
                ecl_device_info  /* param_name */,
                size_t          /* param_value_size */,
                void *          /* param_value */,
                size_t *        /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_2

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclCreateSubDevices(ecl_device_id                         /* in_device */,
                   const ecl_device_partition_property * /* properties */,
                   ecl_uint                              /* num_devices */,
                   ecl_device_id *                       /* out_devices */,
                   ecl_uint *                            /* num_devices_ret */) ECL_API_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainDevice(ecl_device_id /* device */) ECL_API_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseDevice(ecl_device_id /* device */) ECL_API_SUFFIX__VERSION_1_2;

#endif

#ifdef ECL_VERSION_2_1

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetDefaultDeviceCommandQueue(ecl_context           /* context */,
                               ecl_device_id         /* device */,
                               ecl_command_queue     /* command_queue */) ECL_API_SUFFIX__VERSION_2_1;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetDeviceAndHostTimer(ecl_device_id    /* device */,
                        ecl_ulong*       /* device_timestamp */,
                        ecl_ulong*       /* host_timestamp */) ECL_API_SUFFIX__VERSION_2_1;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetHostTimer(ecl_device_id /* device */,
               ecl_ulong *   /* host_timestamp */)  ECL_API_SUFFIX__VERSION_2_1;

#endif

/* Context APIs  */
extern ECL_API_ENTRY ecl_context ECL_API_CALL
eclCreateContext(const ecl_context_properties * /* properties */,
                ecl_uint                 /* num_devices */,
                const ecl_device_id *    /* devices */,
                void (ECL_CALLBACK * /* pfn_notify */)(const char *, const void *, size_t, void *),
                void *                  /* user_data */,
                ecl_int *                /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_context ECL_API_CALL
eclCreateContextFromType(const ecl_context_properties * /* properties */,
                        ecl_device_type          /* device_type */,
                        void (ECL_CALLBACK *     /* pfn_notify*/ )(const char *, const void *, size_t, void *),
                        void *                  /* user_data */,
                        ecl_int *                /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainContext(ecl_context /* context */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseContext(ecl_context /* context */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetContextInfo(ecl_context         /* context */,
                 ecl_context_info    /* param_name */,
                 size_t             /* param_value_size */,
                 void *             /* param_value */,
                 size_t *           /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

/* Command Queue APIs */

#ifdef ECL_VERSION_2_0

extern ECL_API_ENTRY ecl_command_queue ECL_API_CALL
eclCreateCommandQueueWithProperties(ecl_context               /* context */,
                                   ecl_device_id             /* device */,
                                   const ecl_queue_properties *    /* properties */,
                                   ecl_int *                 /* errcode_ret */) ECL_API_SUFFIX__VERSION_2_0;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainCommandQueue(ecl_command_queue /* command_queue */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseCommandQueue(ecl_command_queue /* command_queue */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetCommandQueueInfo(ecl_command_queue      /* command_queue */,
                      ecl_command_queue_info /* param_name */,
                      size_t                /* param_value_size */,
                      void *                /* param_value */,
                      size_t *              /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

/* Memory Object APIs */
extern ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateBuffer(ecl_context   /* context */,
               ecl_mem_flags /* flags */,
               size_t       /* size */,
               void *       /* host_ptr */,
               ecl_int *     /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern  ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateBufferFromDmaBuf(ecl_context   /* context */,
                          ecl_mem_flags /* flags */,
                          ecl_int       /* fd */,
                          size_t        /* size */,
                          ecl_int *     /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateDMABuffer(ecl_context   /* context */,
               ecl_mem_flags /* flags */,
               size_t       /* size */,
               void *       /* host_ptr */,
               ecl_int *     /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_1

extern ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateSubBuffer(ecl_mem                   /* buffer */,
                  ecl_mem_flags             /* flags */,
                  ecl_buffer_create_type    /* buffer_create_type */,
                  const void *             /* buffer_create_info */,
                  ecl_int *                 /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_1;

#endif

#ifdef ECL_VERSION_1_2

extern ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateImage(ecl_context              /* context */,
              ecl_mem_flags            /* flags */,
              const ecl_image_format * /* image_format */,
              const ecl_image_desc *   /* image_desc */,
              void *                  /* host_ptr */,
              ecl_int *                /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_2;

#endif

#ifdef ECL_VERSION_2_0

extern ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreatePipe(ecl_context                 /* context */,
             ecl_mem_flags               /* flags */,
             ecl_uint                    /* pipe_packet_size */,
             ecl_uint                    /* pipe_max_packets */,
             const ecl_pipe_properties * /* properties */,
             ecl_int *                   /* errcode_ret */) ECL_API_SUFFIX__VERSION_2_0;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainMemObject(ecl_mem /* memobj */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseMemObject(ecl_mem /* memobj */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetSupportedImageFormats(ecl_context           /* context */,
                           ecl_mem_flags         /* flags */,
                           ecl_mem_object_type   /* image_type */,
                           ecl_uint              /* num_entries */,
                           ecl_image_format *    /* image_formats */,
                           ecl_uint *            /* num_image_formats */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetMemObjectInfo(ecl_mem           /* memobj */,
                   ecl_mem_info      /* param_name */,
                   size_t           /* param_value_size */,
                   void *           /* param_value */,
                   size_t *         /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetImageInfo(ecl_mem           /* image */,
               ecl_image_info    /* param_name */,
               size_t           /* param_value_size */,
               void *           /* param_value */,
               size_t *         /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_2_0

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetPipeInfo(ecl_mem           /* pipe */,
              ecl_pipe_info     /* param_name */,
              size_t           /* param_value_size */,
              void *           /* param_value */,
              size_t *         /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_2_0;

#endif

#ifdef ECL_VERSION_1_1

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetMemObjectDestructorCallback(ecl_mem /* memobj */,
                                 void (ECL_CALLBACK * /*pfn_notify*/)( ecl_mem /* memobj */, void* /*user_data*/),
                                 void * /*user_data */ )             ECL_API_SUFFIX__VERSION_1_1;

#endif

/* SVM Allocation APIs */

#ifdef ECL_VERSION_2_0

extern ECL_API_ENTRY void * ECL_API_CALL
eclSVMAlloc(ecl_context       /* context */,
           ecl_svm_mem_flags /* flags */,
           size_t           /* size */,
           ecl_uint          /* alignment */) ECL_API_SUFFIX__VERSION_2_0;

extern ECL_API_ENTRY void ECL_API_CALL
eclSVMFree(ecl_context        /* context */,
          void *            /* svm_pointer */) ECL_API_SUFFIX__VERSION_2_0;

#endif

/* Sampler APIs */

#ifdef ECL_VERSION_2_0

extern ECL_API_ENTRY ecl_sampler ECL_API_CALL
eclCreateSamplerWithProperties(ecl_context                     /* context */,
                              const ecl_sampler_properties *  /* sampler_properties */,
                              ecl_int *                       /* errcode_ret */) ECL_API_SUFFIX__VERSION_2_0;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainSampler(ecl_sampler /* sampler */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseSampler(ecl_sampler /* sampler */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetSamplerInfo(ecl_sampler         /* sampler */,
                 ecl_sampler_info    /* param_name */,
                 size_t             /* param_value_size */,
                 void *             /* param_value */,
                 size_t *           /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

/* Program Object APIs  */
extern ECL_API_ENTRY ecl_program ECL_API_CALL
eclCreateProgramWithSource(ecl_context        /* context */,
                          ecl_uint           /* count */,
                          const char **     /* strings */,
                          const size_t *    /* lengths */,
                          ecl_int *          /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_program ECL_API_CALL
eclCreateProgramWithBinary(ecl_context                     /* context */,
                          ecl_uint                        /* num_devices */,
                          const ecl_device_id *           /* device_list */,
                          const size_t *                 /* lengths */,
                          const unsigned char **         /* binaries */,
                          ecl_int *                       /* binary_status */,
                          ecl_int *                       /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_program ECL_API_CALL
eclCreateProgramWithSharedSections(ecl_context                     /* context */,
                                   ecl_uint                        /* num_devices */,
                                   const ecl_device_id *           /* device_list */,
                                   const size_t *                 /* lengths */,
                                   const unsigned char **         /* binaries */,
                                   ecl_int *                       /* binary_status */,
                                   ecl_uint                        /* num_shared_sections */,
                                   const ecl_shared_section *      /* shared_sections */,
                                   ecl_int *                       /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_2

extern ECL_API_ENTRY ecl_program ECL_API_CALL
eclCreateProgramWithBuiltInKernels(ecl_context            /* context */,
                                  ecl_uint               /* num_devices */,
                                  const ecl_device_id *  /* device_list */,
                                  const char *          /* kernel_names */,
                                  ecl_int *              /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_2;

#endif

#ifdef ECL_VERSION_2_1

extern ECL_API_ENTRY ecl_program ECL_API_CALL
eclCreateProgramWithIL(ecl_context    /* context */,
                     const void*    /* il */,
                     size_t         /* length */,
                     ecl_int*        /* errcode_ret */) ECL_API_SUFFIX__VERSION_2_1;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainProgram(ecl_program /* program */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseProgram(ecl_program /* program */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclBuildProgram(ecl_program           /* program */,
               ecl_uint              /* num_devices */,
               const ecl_device_id * /* device_list */,
               const char *         /* options */,
               void (ECL_CALLBACK *  /* pfn_notify */)(ecl_program /* program */, void * /* user_data */),
               void *               /* user_data */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_2

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclCompileProgram(ecl_program           /* program */,
                 ecl_uint              /* num_devices */,
                 const ecl_device_id * /* device_list */,
                 const char *         /* options */,
                 ecl_uint              /* num_input_headers */,
                 const ecl_program *   /* input_headers */,
                 const char **        /* header_include_names */,
                 void (ECL_CALLBACK *  /* pfn_notify */)(ecl_program /* program */, void * /* user_data */),
                 void *               /* user_data */) ECL_API_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_program ECL_API_CALL
eclLinkProgram(ecl_context           /* context */,
              ecl_uint              /* num_devices */,
              const ecl_device_id * /* device_list */,
              const char *         /* options */,
              ecl_uint              /* num_input_programs */,
              const ecl_program *   /* input_programs */,
              void (ECL_CALLBACK *  /* pfn_notify */)(ecl_program /* program */, void * /* user_data */),
              void *               /* user_data */,
              ecl_int *             /* errcode_ret */ ) ECL_API_SUFFIX__VERSION_1_2;

#endif

#ifdef ECL_VERSION_2_2

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetProgramReleaseCallback(ecl_program          /* program */,
                            void (ECL_CALLBACK * /* pfn_notify */)(ecl_program /* program */, void * /* user_data */),
                            void *              /* user_data */) ECL_API_SUFFIX__VERSION_2_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetProgramSpecializationConstant(ecl_program  /* program */,
                                   ecl_uint     /* spec_id */,
                                   size_t      /* spec_size */,
                                   const void* /* spec_value */) ECL_API_SUFFIX__VERSION_2_2;

#endif

#ifdef ECL_VERSION_1_2

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclUnloadPlatformCompiler(ecl_platform_id /* platform */) ECL_API_SUFFIX__VERSION_1_2;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetProgramInfo(ecl_program         /* program */,
                 ecl_program_info    /* param_name */,
                 size_t             /* param_value_size */,
                 void *             /* param_value */,
                 size_t *           /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetProgramBuildInfo(ecl_program            /* program */,
                      ecl_device_id          /* device */,
                      ecl_program_build_info /* param_name */,
                      size_t                /* param_value_size */,
                      void *                /* param_value */,
                      size_t *              /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

/* Kernel Object APIs */
extern ECL_API_ENTRY ecl_kernel ECL_API_CALL
eclCreateKernel(ecl_program      /* program */,
               const char *    /* kernel_name */,
               ecl_int *        /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclCreateKernelsInProgram(ecl_program     /* program */,
                         ecl_uint        /* num_kernels */,
                         ecl_kernel *    /* kernels */,
                         ecl_uint *      /* num_kernels_ret */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_2_1

extern ECL_API_ENTRY ecl_kernel ECL_API_CALL
eclCloneKernel(ecl_kernel     /* source_kernel */,
              ecl_int*       /* errcode_ret */) ECL_API_SUFFIX__VERSION_2_1;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainKernel(ecl_kernel    /* kernel */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseKernel(ecl_kernel   /* kernel */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelArg(ecl_kernel    /* kernel */,
               ecl_uint      /* arg_index */,
               size_t       /* arg_size */,
               const void * /* arg_value */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_2_0

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelArgSVMPointer(ecl_kernel    /* kernel */,
                         ecl_uint      /* arg_index */,
                         const void * /* arg_value */) ECL_API_SUFFIX__VERSION_2_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelExecInfo(ecl_kernel            /* kernel */,
                    ecl_kernel_exec_info  /* param_name */,
                    size_t               /* param_value_size */,
                    const void *         /* param_value */) ECL_API_SUFFIX__VERSION_2_0;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetKernelInfo(ecl_kernel       /* kernel */,
                ecl_kernel_info  /* param_name */,
                size_t          /* param_value_size */,
                void *          /* param_value */,
                size_t *        /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_2

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetKernelArgInfo(ecl_kernel       /* kernel */,
                   ecl_uint         /* arg_indx */,
                   ecl_kernel_arg_info  /* param_name */,
                   size_t          /* param_value_size */,
                   void *          /* param_value */,
                   size_t *        /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_2;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetKernelWorkGroupInfo(ecl_kernel                  /* kernel */,
                         ecl_device_id               /* device */,
                         ecl_kernel_work_group_info  /* param_name */,
                         size_t                     /* param_value_size */,
                         void *                     /* param_value */,
                         size_t *                   /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_2_1

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetKernelSubGroupInfo(ecl_kernel                   /* kernel */,
                        ecl_device_id                /* device */,
                        ecl_kernel_sub_group_info    /* param_name */,
                        size_t                      /* input_value_size */,
                        const void*                 /*input_value */,
                        size_t                      /* param_value_size */,
                        void*                       /* param_value */,
                        size_t*                     /* param_value_size_ret */ ) ECL_API_SUFFIX__VERSION_2_1;

#endif

/* Event Object APIs */
extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclWaitForEvents(ecl_uint             /* num_events */,
                const ecl_event *    /* event_list */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetEventInfo(ecl_event         /* event */,
               ecl_event_info    /* param_name */,
               size_t           /* param_value_size */,
               void *           /* param_value */,
               size_t *         /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_1

extern ECL_API_ENTRY ecl_event ECL_API_CALL
eclCreateUserEvent(ecl_context    /* context */,
                  ecl_int *      /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_1;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainEvent(ecl_event /* event */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseEvent(ecl_event /* event */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_1

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetUserEventStatus(ecl_event   /* event */,
                     ecl_int     /* execution_status */) ECL_API_SUFFIX__VERSION_1_1;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetEventCallback( ecl_event    /* event */,
                    ecl_int      /* command_exec_callback_type */,
                    void (ECL_CALLBACK * /* pfn_notify */)(ecl_event, ecl_int, void *),
                    void *      /* user_data */) ECL_API_SUFFIX__VERSION_1_1;

#endif

/* Profiling APIs */
extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetEventProfilingInfo(ecl_event            /* event */,
                        ecl_profiling_info   /* param_name */,
                        size_t              /* param_value_size */,
                        void *              /* param_value */,
                        size_t *            /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

/* Flush and Finish APIs */
extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclFlush(ecl_command_queue /* command_queue */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclFinish(ecl_command_queue /* command_queue */) ECL_API_SUFFIX__VERSION_1_0;

/* Enqueued Commands APIs */
extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueReadBuffer(ecl_command_queue    /* command_queue */,
                    ecl_mem              /* buffer */,
                    ecl_bool             /* blocking_read */,
                    size_t              /* offset */,
                    size_t              /* size */,
                    void *              /* ptr */,
                    ecl_uint             /* num_events_in_wait_list */,
                    const ecl_event *    /* event_wait_list */,
                    ecl_event *          /* event */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_1

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueReadBufferRect(ecl_command_queue    /* command_queue */,
                        ecl_mem              /* buffer */,
                        ecl_bool             /* blocking_read */,
                        const size_t *      /* buffer_offset */,
                        const size_t *      /* host_offset */,
                        const size_t *      /* region */,
                        size_t              /* buffer_row_pitch */,
                        size_t              /* buffer_slice_pitch */,
                        size_t              /* host_row_pitch */,
                        size_t              /* host_slice_pitch */,
                        void *              /* ptr */,
                        ecl_uint             /* num_events_in_wait_list */,
                        const ecl_event *    /* event_wait_list */,
                        ecl_event *          /* event */) ECL_API_SUFFIX__VERSION_1_1;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueWriteBuffer(ecl_command_queue   /* command_queue */,
                     ecl_mem             /* buffer */,
                     ecl_bool            /* blocking_write */,
                     size_t             /* offset */,
                     size_t             /* size */,
                     const void *       /* ptr */,
                     ecl_uint            /* num_events_in_wait_list */,
                     const ecl_event *   /* event_wait_list */,
                     ecl_event *         /* event */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_1

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueWriteBufferRect(ecl_command_queue    /* command_queue */,
                         ecl_mem              /* buffer */,
                         ecl_bool             /* blocking_write */,
                         const size_t *      /* buffer_offset */,
                         const size_t *      /* host_offset */,
                         const size_t *      /* region */,
                         size_t              /* buffer_row_pitch */,
                         size_t              /* buffer_slice_pitch */,
                         size_t              /* host_row_pitch */,
                         size_t              /* host_slice_pitch */,
                         const void *        /* ptr */,
                         ecl_uint             /* num_events_in_wait_list */,
                         const ecl_event *    /* event_wait_list */,
                         ecl_event *          /* event */) ECL_API_SUFFIX__VERSION_1_1;

#endif

#ifdef ECL_VERSION_1_2

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueFillBuffer(ecl_command_queue   /* command_queue */,
                    ecl_mem             /* buffer */,
                    const void *       /* pattern */,
                    size_t             /* pattern_size */,
                    size_t             /* offset */,
                    size_t             /* size */,
                    ecl_uint            /* num_events_in_wait_list */,
                    const ecl_event *   /* event_wait_list */,
                    ecl_event *         /* event */) ECL_API_SUFFIX__VERSION_1_2;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueCopyBuffer(ecl_command_queue    /* command_queue */,
                    ecl_mem              /* src_buffer */,
                    ecl_mem              /* dst_buffer */,
                    size_t              /* src_offset */,
                    size_t              /* dst_offset */,
                    size_t              /* size */,
                    ecl_uint             /* num_events_in_wait_list */,
                    const ecl_event *    /* event_wait_list */,
                    ecl_event *          /* event */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_1

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueCopyBufferRect(ecl_command_queue    /* command_queue */,
                        ecl_mem              /* src_buffer */,
                        ecl_mem              /* dst_buffer */,
                        const size_t *      /* src_origin */,
                        const size_t *      /* dst_origin */,
                        const size_t *      /* region */,
                        size_t              /* src_row_pitch */,
                        size_t              /* src_slice_pitch */,
                        size_t              /* dst_row_pitch */,
                        size_t              /* dst_slice_pitch */,
                        ecl_uint             /* num_events_in_wait_list */,
                        const ecl_event *    /* event_wait_list */,
                        ecl_event *          /* event */) ECL_API_SUFFIX__VERSION_1_1;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueReadImage(ecl_command_queue     /* command_queue */,
                   ecl_mem               /* image */,
                   ecl_bool              /* blocking_read */,
                   const size_t *       /* origin[3] */,
                   const size_t *       /* region[3] */,
                   size_t               /* row_pitch */,
                   size_t               /* slice_pitch */,
                   void *               /* ptr */,
                   ecl_uint              /* num_events_in_wait_list */,
                   const ecl_event *     /* event_wait_list */,
                   ecl_event *           /* event */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueWriteImage(ecl_command_queue    /* command_queue */,
                    ecl_mem              /* image */,
                    ecl_bool             /* blocking_write */,
                    const size_t *      /* origin[3] */,
                    const size_t *      /* region[3] */,
                    size_t              /* input_row_pitch */,
                    size_t              /* input_slice_pitch */,
                    const void *        /* ptr */,
                    ecl_uint             /* num_events_in_wait_list */,
                    const ecl_event *    /* event_wait_list */,
                    ecl_event *          /* event */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_2

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueFillImage(ecl_command_queue   /* command_queue */,
                   ecl_mem             /* image */,
                   const void *       /* fill_color */,
                   const size_t *     /* origin[3] */,
                   const size_t *     /* region[3] */,
                   ecl_uint            /* num_events_in_wait_list */,
                   const ecl_event *   /* event_wait_list */,
                   ecl_event *         /* event */) ECL_API_SUFFIX__VERSION_1_2;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueCopyImage(ecl_command_queue     /* command_queue */,
                   ecl_mem               /* src_image */,
                   ecl_mem               /* dst_image */,
                   const size_t *       /* src_origin[3] */,
                   const size_t *       /* dst_origin[3] */,
                   const size_t *       /* region[3] */,
                   ecl_uint              /* num_events_in_wait_list */,
                   const ecl_event *     /* event_wait_list */,
                   ecl_event *           /* event */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueCopyImageToBuffer(ecl_command_queue /* command_queue */,
                           ecl_mem           /* src_image */,
                           ecl_mem           /* dst_buffer */,
                           const size_t *   /* src_origin[3] */,
                           const size_t *   /* region[3] */,
                           size_t           /* dst_offset */,
                           ecl_uint          /* num_events_in_wait_list */,
                           const ecl_event * /* event_wait_list */,
                           ecl_event *       /* event */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueCopyBufferToImage(ecl_command_queue /* command_queue */,
                           ecl_mem           /* src_buffer */,
                           ecl_mem           /* dst_image */,
                           size_t           /* src_offset */,
                           const size_t *   /* dst_origin[3] */,
                           const size_t *   /* region[3] */,
                           ecl_uint          /* num_events_in_wait_list */,
                           const ecl_event * /* event_wait_list */,
                           ecl_event *       /* event */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY void * ECL_API_CALL
eclEnqueueMapBuffer(ecl_command_queue /* command_queue */,
                   ecl_mem           /* buffer */,
                   ecl_bool          /* blocking_map */,
                   ecl_map_flags     /* map_flags */,
                   size_t           /* offset */,
                   size_t           /* cb */,
                   ecl_uint          /* num_events_in_wait_list */,
                   const ecl_event * /* event_wait_list */,
                   ecl_event *       /* event */,
                   ecl_int *         /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY void * ECL_API_CALL
eclEnqueueMapImage(ecl_command_queue  /* command_queue */,
                  ecl_mem            /* image */,
                  ecl_bool           /* blocking_map */,
                  ecl_map_flags      /* map_flags */,
                  const size_t *    /* origin[3] */,
                  const size_t *    /* region[3] */,
                  size_t *          /* image_row_pitch */,
                  size_t *          /* image_slice_pitch */,
                  ecl_uint           /* num_events_in_wait_list */,
                  const ecl_event *  /* event_wait_list */,
                  ecl_event *        /* event */,
                  ecl_int *          /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueUnmapMemObject(ecl_command_queue /* command_queue */,
                        ecl_mem           /* memobj */,
                        void *           /* mapped_ptr */,
                        ecl_uint          /* num_events_in_wait_list */,
                        const ecl_event *  /* event_wait_list */,
                        ecl_event *        /* event */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_2

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueMigrateMemObjects(ecl_command_queue       /* command_queue */,
                           ecl_uint                /* num_mem_objects */,
                           const ecl_mem *         /* mem_objects */,
                           ecl_mem_migration_flags /* flags */,
                           ecl_uint                /* num_events_in_wait_list */,
                           const ecl_event *       /* event_wait_list */,
                           ecl_event *             /* event */) ECL_API_SUFFIX__VERSION_1_2;

#endif

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueNDRangeKernel(ecl_command_queue /* command_queue */,
                       ecl_kernel        /* kernel */,
                       ecl_uint          /* work_dim */,
                       const size_t *   /* global_work_offset */,
                       const size_t *   /* global_work_size */,
                       const size_t *   /* local_work_size */,
                       ecl_uint          /* num_events_in_wait_list */,
                       const ecl_event * /* event_wait_list */,
                       ecl_event *       /* event */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueNativeKernel(ecl_command_queue  /* command_queue */,
                      void (ECL_CALLBACK * /*user_func*/)(void *),
                      void *            /* args */,
                      size_t            /* cb_args */,
                      ecl_uint           /* num_mem_objects */,
                      const ecl_mem *    /* mem_list */,
                      const void **     /* args_mem_loc */,
                      ecl_uint           /* num_events_in_wait_list */,
                      const ecl_event *  /* event_wait_list */,
                      ecl_event *        /* event */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_2

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueMarkerWithWaitList(ecl_command_queue  /* command_queue */,
                            ecl_uint           /* num_events_in_wait_list */,
                            const ecl_event *  /* event_wait_list */,
                            ecl_event *        /* event */) ECL_API_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueBarrierWithWaitList(ecl_command_queue  /* command_queue */,
                             ecl_uint           /* num_events_in_wait_list */,
                             const ecl_event *  /* event_wait_list */,
                             ecl_event *        /* event */) ECL_API_SUFFIX__VERSION_1_2;

#endif

#ifdef ECL_VERSION_2_0

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueSVMFree(ecl_command_queue  /* command_queue */,
                 ecl_uint           /* num_svm_pointers */,
                 void *[]          /* svm_pointers[] */,
                 void (ECL_CALLBACK * /*pfn_free_func*/)(ecl_command_queue /* queue */,
                                                        ecl_uint          /* num_svm_pointers */,
                                                        void *[]         /* svm_pointers[] */,
                                                        void *           /* user_data */),
                 void *            /* user_data */,
                 ecl_uint           /* num_events_in_wait_list */,
                 const ecl_event *  /* event_wait_list */,
                 ecl_event *        /* event */) ECL_API_SUFFIX__VERSION_2_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueSVMMemcpy(ecl_command_queue  /* command_queue */,
                   ecl_bool           /* blocking_copy */,
                   void *            /* dst_ptr */,
                   const void *      /* src_ptr */,
                   size_t            /* size */,
                   ecl_uint           /* num_events_in_wait_list */,
                   const ecl_event *  /* event_wait_list */,
                   ecl_event *        /* event */) ECL_API_SUFFIX__VERSION_2_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueSVMMemFill(ecl_command_queue  /* command_queue */,
                    void *            /* svm_ptr */,
                    const void *      /* pattern */,
                    size_t            /* pattern_size */,
                    size_t            /* size */,
                    ecl_uint           /* num_events_in_wait_list */,
                    const ecl_event *  /* event_wait_list */,
                    ecl_event *        /* event */) ECL_API_SUFFIX__VERSION_2_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueSVMMap(ecl_command_queue  /* command_queue */,
                ecl_bool           /* blocking_map */,
                ecl_map_flags      /* flags */,
                void *            /* svm_ptr */,
                size_t            /* size */,
                ecl_uint           /* num_events_in_wait_list */,
                const ecl_event *  /* event_wait_list */,
                ecl_event *        /* event */) ECL_API_SUFFIX__VERSION_2_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueSVMUnmap(ecl_command_queue  /* command_queue */,
                  void *            /* svm_ptr */,
                  ecl_uint           /* num_events_in_wait_list */,
                  const ecl_event *  /* event_wait_list */,
                  ecl_event *        /* event */) ECL_API_SUFFIX__VERSION_2_0;

#endif

#ifdef ECL_VERSION_2_1

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueSVMMigrateMem(ecl_command_queue         /* command_queue */,
                       ecl_uint                  /* num_svm_pointers */,
                       const void **            /* svm_pointers */,
                       const size_t *           /* sizes */,
                       ecl_mem_migration_flags   /* flags */,
                       ecl_uint                  /* num_events_in_wait_list */,
                       const ecl_event *         /* event_wait_list */,
                       ecl_event *               /* event */) ECL_API_SUFFIX__VERSION_2_1;

#endif

#ifdef ECL_VERSION_1_2

/* Extension function access
 *
 * Returns the extension function address for the given function name,
 * or NULL if a valid function can not be found.  The eclient must
 * check to make sure the address is not NULL, before using or
 * calling the returned function address.
 */
extern ECL_API_ENTRY void * ECL_API_CALL
eclGetExtensionFunctionAddressForPlatform(ecl_platform_id /* platform */,
                                         const char *   /* func_name */) ECL_API_SUFFIX__VERSION_1_2;

#endif

#ifdef ECL_USE_DEPRECATED_OPENCL_1_0_APIS
    /*
     *  WARNING:
     *     This API introduces mutable state into the OpenCL implementation. It has been REMOVED
     *  to better facilitate thread safety.  The 1.0 API is not thread safe. It is not tested by the
     *  OpenCL 1.1 conformance test, and consequently may not work or may not work dependably.
     *  It is likely to be non-performant. Use of this API is not advised. Use at your own risk.
     *
     *  Software developers previously relying on this API are instructed to set the command queue
     *  properties when creating the queue, instead.
     */
    extern ECL_API_ENTRY ecl_int ECL_API_CALL
    eclSetCommandQueueProperty(ecl_command_queue              /* command_queue */,
                              ecl_command_queue_properties   /* properties */,
                              ecl_bool                        /* enable */,
                              ecl_command_queue_properties * /* old_properties */) ECL_EXT_SUFFIX__VERSION_1_0_DEPRECATED;
#endif /* ECL_USE_DEPRECATED_OPENCL_1_0_APIS */

/* Deprecated OpenCL 1.1 APIs */
extern ECL_API_ENTRY ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED ecl_mem ECL_API_CALL
eclCreateImage2D(ecl_context              /* context */,
                ecl_mem_flags            /* flags */,
                const ecl_image_format * /* image_format */,
                size_t                  /* image_width */,
                size_t                  /* image_height */,
                size_t                  /* image_row_pitch */,
                void *                  /* host_ptr */,
                ecl_int *                /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern ECL_API_ENTRY ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED ecl_mem ECL_API_CALL
eclCreateImage3D(ecl_context              /* context */,
                ecl_mem_flags            /* flags */,
                const ecl_image_format * /* image_format */,
                size_t                  /* image_width */,
                size_t                  /* image_height */,
                size_t                  /* image_depth */,
                size_t                  /* image_row_pitch */,
                size_t                  /* image_slice_pitch */,
                void *                  /* host_ptr */,
                ecl_int *                /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern ECL_API_ENTRY ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED ecl_int ECL_API_CALL
eclEnqueueMarker(ecl_command_queue    /* command_queue */,
                ecl_event *          /* event */) ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern ECL_API_ENTRY ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED ecl_int ECL_API_CALL
eclEnqueueWaitForEvents(ecl_command_queue /* command_queue */,
                        ecl_uint          /* num_events */,
                        const ecl_event * /* event_list */) ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern ECL_API_ENTRY ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED ecl_int ECL_API_CALL
eclEnqueueBarrier(ecl_command_queue /* command_queue */) ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern ECL_API_ENTRY ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED ecl_int ECL_API_CALL
eclUnloadCompiler(void) ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern ECL_API_ENTRY ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED void * ECL_API_CALL
eclGetExtensionFunctionAddress(const char * /* func_name */) ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

/* Deprecated OpenCL 2.0 APIs */
extern ECL_API_ENTRY ECL_EXT_PREFIX__VERSION_1_2_DEPRECATED ecl_command_queue ECL_API_CALL
eclCreateCommandQueue(ecl_context                     /* context */,
                     ecl_device_id                   /* device */,
                     ecl_command_queue_properties    /* properties */,
                     ecl_int *                       /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_2_DEPRECATED;

extern ECL_API_ENTRY ECL_EXT_PREFIX__VERSION_1_2_DEPRECATED ecl_sampler ECL_API_CALL
eclCreateSampler(ecl_context          /* context */,
                ecl_bool             /* normalized_coords */,
                ecl_addressing_mode  /* addressing_mode */,
                ecl_filter_mode      /* filter_mode */,
                ecl_int *            /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_2_DEPRECATED;

extern ECL_API_ENTRY ECL_EXT_PREFIX__VERSION_1_2_DEPRECATED ecl_int ECL_API_CALL
eclEnqueueTask(ecl_command_queue  /* command_queue */,
              ecl_kernel         /* kernel */,
              ecl_uint           /* num_events_in_wait_list */,
              const ecl_event *  /* event_wait_list */,
              ecl_event *        /* event */) ECL_EXT_SUFFIX__VERSION_1_2_DEPRECATED;

#ifdef __cplusplus
}
#endif

#endif  /* __ECL_H */
