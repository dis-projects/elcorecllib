// Copyright 2018-2019 RnD Center "ELVEES", JSC
/*******************************************************************************
 * Copyright (c) 2008-2017 The Khronos Group Inc.
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
/*****************************************************************************\

Copyright (c) 2013-2017 Intel Corporation All Rights Reserved.

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

File Name: ecl_ext_intel.h

Abstract:

Notes:

\*****************************************************************************/

#ifndef __ELCORE_ECL_EXT_INTEL_H
#define __ELCORE_ECL_EXT_INTEL_H

#include <elcorecl/ecl.h>
#include <elcorecl/ecl_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
* ecl_intel_thread_local_exec extension *
****************************************/

#define ecl_intel_thread_local_exec 1

#define ECL_QUEUE_THREAD_LOCAL_EXEC_ENABLE_INTEL      (((ecl_bitfield)1) << 31)

/***********************************************
* ecl_intel_device_partition_by_names extension *
************************************************/

#define ecl_intel_device_partition_by_names 1

#define ECL_DEVICE_PARTITION_BY_NAMES_INTEL          0x4052
#define ECL_PARTITION_BY_NAMES_LIST_END_INTEL        -1

/************************************************
* ecl_intel_accelerator extension                *
* ecl_intel_motion_estimation extension          *
* ecl_intel_advanced_motion_estimation extension *
*************************************************/

#define ecl_intel_accelerator 1
#define ecl_intel_motion_estimation 1
#define ecl_intel_advanced_motion_estimation 1

typedef struct _ecl_accelerator_intel* ecl_accelerator_intel;
typedef ecl_uint ecl_accelerator_type_intel;
typedef ecl_uint ecl_accelerator_info_intel;

typedef struct _ecl_motion_estimation_desc_intel {
    ecl_uint mb_block_type;
    ecl_uint subpixel_mode;
    ecl_uint sad_adjust_mode;
    ecl_uint search_path_type;
} ecl_motion_estimation_desc_intel;

/* error codes */
#define ECL_INVALID_ACCELERATOR_INTEL                              -1094
#define ECL_INVALID_ACCELERATOR_TYPE_INTEL                         -1095
#define ECL_INVALID_ACCELERATOR_DESCRIPTOR_INTEL                   -1096
#define ECL_ACCELERATOR_TYPE_NOT_SUPPORTED_INTEL                   -1097

/* ecl_accelerator_type_intel */
#define ECL_ACCELERATOR_TYPE_MOTION_ESTIMATION_INTEL               0x0

/* ecl_accelerator_info_intel */
#define ECL_ACCELERATOR_DESCRIPTOR_INTEL                           0x4090
#define ECL_ACCELERATOR_REFERENCE_COUNT_INTEL                      0x4091
#define ECL_ACCELERATOR_CONTEXT_INTEL                              0x4092
#define ECL_ACCELERATOR_TYPE_INTEL                                 0x4093

/* ecl_motion_detect_desc_intel flags */
#define ECL_ME_MB_TYPE_16x16_INTEL                                 0x0
#define ECL_ME_MB_TYPE_8x8_INTEL                                   0x1
#define ECL_ME_MB_TYPE_4x4_INTEL                                   0x2

#define ECL_ME_SUBPIXEL_MODE_INTEGER_INTEL                         0x0
#define ECL_ME_SUBPIXEL_MODE_HPEL_INTEL                            0x1
#define ECL_ME_SUBPIXEL_MODE_QPEL_INTEL                            0x2

#define ECL_ME_SAD_ADJUST_MODE_NONE_INTEL                          0x0
#define ECL_ME_SAD_ADJUST_MODE_HAAR_INTEL                          0x1

#define ECL_ME_SEARCH_PATH_RADIUS_2_2_INTEL                        0x0
#define ECL_ME_SEARCH_PATH_RADIUS_4_4_INTEL                        0x1
#define ECL_ME_SEARCH_PATH_RADIUS_16_12_INTEL                      0x5

#define ECL_ME_SKIP_BLOCK_TYPE_16x16_INTEL                         0x0
#define ECL_ME_CHROMA_INTRA_PREDICT_ENABLED_INTEL                  0x1
#define ECL_ME_LUMA_INTRA_PREDICT_ENABLED_INTEL                    0x2
#define ECL_ME_SKIP_BLOCK_TYPE_8x8_INTEL                           0x4

#define ECL_ME_FORWARD_INPUT_MODE_INTEL                            0x1
#define ECL_ME_BACKWARD_INPUT_MODE_INTEL                           0x2
#define ECL_ME_BIDIRECTION_INPUT_MODE_INTEL                        0x3

#define ECL_ME_BIDIR_WEIGHT_QUARTER_INTEL                          16
#define ECL_ME_BIDIR_WEIGHT_THIRD_INTEL                            21
#define ECL_ME_BIDIR_WEIGHT_HALF_INTEL                             32
#define ECL_ME_BIDIR_WEIGHT_TWO_THIRD_INTEL                        43
#define ECL_ME_BIDIR_WEIGHT_THREE_QUARTER_INTEL                    48

#define ECL_ME_COST_PENALTY_NONE_INTEL                             0x0
#define ECL_ME_COST_PENALTY_LOW_INTEL                              0x1
#define ECL_ME_COST_PENALTY_NORMAL_INTEL                           0x2
#define ECL_ME_COST_PENALTY_HIGH_INTEL                             0x3

#define ECL_ME_COST_PRECISION_QPEL_INTEL                           0x0
#define ECL_ME_COST_PRECISION_HPEL_INTEL                           0x1
#define ECL_ME_COST_PRECISION_PEL_INTEL                            0x2
#define ECL_ME_COST_PRECISION_DPEL_INTEL                           0x3

#define ECL_ME_LUMA_PREDICTOR_MODE_VERTICAL_INTEL                  0x0
#define ECL_ME_LUMA_PREDICTOR_MODE_HORIZONTAL_INTEL                0x1
#define ECL_ME_LUMA_PREDICTOR_MODE_DC_INTEL                        0x2
#define ECL_ME_LUMA_PREDICTOR_MODE_DIAGONAL_DOWN_LEFT_INTEL        0x3

#define ECL_ME_LUMA_PREDICTOR_MODE_DIAGONAL_DOWN_RIGHT_INTEL       0x4
#define ECL_ME_LUMA_PREDICTOR_MODE_PLANE_INTEL                     0x4
#define ECL_ME_LUMA_PREDICTOR_MODE_VERTICAL_RIGHT_INTEL            0x5
#define ECL_ME_LUMA_PREDICTOR_MODE_HORIZONTAL_DOWN_INTEL           0x6
#define ECL_ME_LUMA_PREDICTOR_MODE_VERTICAL_LEFT_INTEL             0x7
#define ECL_ME_LUMA_PREDICTOR_MODE_HORIZONTAL_UP_INTEL             0x8

#define ECL_ME_CHROMA_PREDICTOR_MODE_DC_INTEL                      0x0
#define ECL_ME_CHROMA_PREDICTOR_MODE_HORIZONTAL_INTEL              0x1
#define ECL_ME_CHROMA_PREDICTOR_MODE_VERTICAL_INTEL                0x2
#define ECL_ME_CHROMA_PREDICTOR_MODE_PLANE_INTEL                   0x3

/* ecl_device_info */
#define ECL_DEVICE_ME_VERSION_INTEL                                0x407E

#define ECL_ME_VERSION_LEGACY_INTEL                                0x0
#define ECL_ME_VERSION_ADVANCED_VER_1_INTEL                        0x1
#define ECL_ME_VERSION_ADVANCED_VER_2_INTEL                        0x2

extern ECL_API_ENTRY ecl_accelerator_intel ECL_API_CALL
eclCreateAcceleratorINTEL(
    ecl_context                  /* context */,
    ecl_accelerator_type_intel   /* accelerator_type */,
    size_t                      /* descriptor_size */,
    const void*                 /* descriptor */,
    ecl_int*                     /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_accelerator_intel (ECL_API_CALL *eclCreateAcceleratorINTEL_fn)(
    ecl_context                  /* context */,
    ecl_accelerator_type_intel   /* accelerator_type */,
    size_t                      /* descriptor_size */,
    const void*                 /* descriptor */,
    ecl_int*                     /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetAcceleratorInfoINTEL(
    ecl_accelerator_intel        /* accelerator */,
    ecl_accelerator_info_intel   /* param_name */,
    size_t                      /* param_value_size */,
    void*                       /* param_value */,
    size_t*                     /* param_value_size_ret */) ECL_EXT_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclGetAcceleratorInfoINTEL_fn)(
    ecl_accelerator_intel        /* accelerator */,
    ecl_accelerator_info_intel   /* param_name */,
    size_t                      /* param_value_size */,
    void*                       /* param_value */,
    size_t*                     /* param_value_size_ret */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainAcceleratorINTEL(
    ecl_accelerator_intel        /* accelerator */) ECL_EXT_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclRetainAcceleratorINTEL_fn)(
    ecl_accelerator_intel        /* accelerator */) ECL_EXT_SUFFIX__VERSION_1_2;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseAcceleratorINTEL(
    ecl_accelerator_intel        /* accelerator */) ECL_EXT_SUFFIX__VERSION_1_2;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclReleaseAcceleratorINTEL_fn)(
    ecl_accelerator_intel        /* accelerator */) ECL_EXT_SUFFIX__VERSION_1_2;

/******************************************
* ecl_intel_simultaneous_sharing extension *
*******************************************/

#define ecl_intel_simultaneous_sharing 1

#define ECL_DEVICE_SIMULTANEOUS_INTEROPS_INTEL            0x4104
#define ECL_DEVICE_NUM_SIMULTANEOUS_INTEROPS_INTEL        0x4105

/***********************************
* ecl_intel_egl_image_yuv extension *
************************************/

#define ecl_intel_egl_image_yuv 1

#define ECL_EGL_YUV_PLANE_INTEL                           0x4107

/********************************
* ecl_intel_packed_yuv extension *
*********************************/

#define ecl_intel_packed_yuv 1

#define ECL_YUYV_INTEL                                    0x4076
#define ECL_UYVY_INTEL                                    0x4077
#define ECL_YVYU_INTEL                                    0x4078
#define ECL_VYUY_INTEL                                    0x4079

/********************************************
* ecl_intel_required_subgroup_size extension *
*********************************************/

#define ecl_intel_required_subgroup_size 1

#define ECL_DEVICE_SUB_GROUP_SIZES_INTEL                  0x4108
#define ECL_KERNEL_SPILL_MEM_SIZE_INTEL                   0x4109
#define ECL_KERNEL_COMPILE_SUB_GROUP_SIZE_INTEL           0x410A

/****************************************
* ecl_intel_driver_diagnostics extension *
*****************************************/

#define ecl_intel_driver_diagnostics 1

typedef ecl_uint ecl_diagnostics_verbose_level;

#define ECL_CONTEXT_SHOW_DIAGNOSTICS_INTEL                0x4106

#define ECL_CONTEXT_DIAGNOSTICS_LEVEL_ALL_INTEL           ( 0xff )
#define ECL_CONTEXT_DIAGNOSTICS_LEVEL_GOOD_INTEL          ( 1 )
#define ECL_CONTEXT_DIAGNOSTICS_LEVEL_BAD_INTEL           ( 1 << 1 )
#define ECL_CONTEXT_DIAGNOSTICS_LEVEL_NEUTRAL_INTEL       ( 1 << 2 )

/********************************
* ecl_intel_planar_yuv extension *
*********************************/

#define ECL_NV12_INTEL                                       0x410E

#define ECL_MEM_NO_ACCESS_INTEL                              ( 1 << 24 )
#define ECL_MEM_ACCESS_FLAGS_UNRESTRICTED_INTEL              ( 1 << 25 )

#define ECL_DEVICE_PLANAR_YUV_MAX_WIDTH_INTEL                0x417E
#define ECL_DEVICE_PLANAR_YUV_MAX_HEIGHT_INTEL               0x417F

/*******************************************************
* ecl_intel_device_side_avc_motion_estimation extension *
********************************************************/

#define ECL_DEVICE_AVC_ME_VERSION_INTEL                      0x410B
#define ECL_DEVICE_AVC_ME_SUPPORTS_TEXTURE_SAMPLER_USE_INTEL 0x410C
#define ECL_DEVICE_AVC_ME_SUPPORTS_PREEMPTION_INTEL          0x410D

#define ECL_AVC_ME_VERSION_0_INTEL                           0x0;  // No support.
#define ECL_AVC_ME_VERSION_1_INTEL                           0x1;  // First supported version.

#define ECL_AVC_ME_MAJOR_16x16_INTEL                         0x0
#define ECL_AVC_ME_MAJOR_16x8_INTEL                          0x1
#define ECL_AVC_ME_MAJOR_8x16_INTEL                          0x2
#define ECL_AVC_ME_MAJOR_8x8_INTEL                           0x3

#define ECL_AVC_ME_MINOR_8x8_INTEL                           0x0
#define ECL_AVC_ME_MINOR_8x4_INTEL                           0x1
#define ECL_AVC_ME_MINOR_4x8_INTEL                           0x2
#define ECL_AVC_ME_MINOR_4x4_INTEL                           0x3

#define ECL_AVC_ME_MAJOR_FORWARD_INTEL                       0x0
#define ECL_AVC_ME_MAJOR_BACKWARD_INTEL                      0x1
#define ECL_AVC_ME_MAJOR_BIDIRECTIONAL_INTEL                 0x2

#define ECL_AVC_ME_PARTITION_MASK_ALL_INTEL                  0x0
#define ECL_AVC_ME_PARTITION_MASK_16x16_INTEL                0x7E
#define ECL_AVC_ME_PARTITION_MASK_16x8_INTEL                 0x7D
#define ECL_AVC_ME_PARTITION_MASK_8x16_INTEL                 0x7B
#define ECL_AVC_ME_PARTITION_MASK_8x8_INTEL                  0x77
#define ECL_AVC_ME_PARTITION_MASK_8x4_INTEL                  0x6F
#define ECL_AVC_ME_PARTITION_MASK_4x8_INTEL                  0x5F
#define ECL_AVC_ME_PARTITION_MASK_4x4_INTEL                  0x3F

#define ECL_AVC_ME_SEARCH_WINDOW_EXHAUSTIVE_INTEL            0x0
#define ECL_AVC_ME_SEARCH_WINDOW_SMALL_INTEL                 0x1
#define ECL_AVC_ME_SEARCH_WINDOW_TINY_INTEL                  0x2
#define ECL_AVC_ME_SEARCH_WINDOW_EXTRA_TINY_INTEL            0x3
#define ECL_AVC_ME_SEARCH_WINDOW_DIAMOND_INTEL               0x4
#define ECL_AVC_ME_SEARCH_WINDOW_LARGE_DIAMOND_INTEL         0x5
#define ECL_AVC_ME_SEARCH_WINDOW_RESERVED0_INTEL             0x6
#define ECL_AVC_ME_SEARCH_WINDOW_RESERVED1_INTEL             0x7
#define ECL_AVC_ME_SEARCH_WINDOW_CUSTOM_INTEL                0x8
#define ECL_AVC_ME_SEARCH_WINDOW_16x12_RADIUS_INTEL          0x9
#define ECL_AVC_ME_SEARCH_WINDOW_4x4_RADIUS_INTEL            0x2
#define ECL_AVC_ME_SEARCH_WINDOW_2x2_RADIUS_INTEL            0xa

#define ECL_AVC_ME_SAD_ADJUST_MODE_NONE_INTEL                0x0
#define ECL_AVC_ME_SAD_ADJUST_MODE_HAAR_INTEL                0x2

#define ECL_AVC_ME_SUBPIXEL_MODE_INTEGER_INTEL               0x0
#define ECL_AVC_ME_SUBPIXEL_MODE_HPEL_INTEL                  0x1
#define ECL_AVC_ME_SUBPIXEL_MODE_QPEL_INTEL                  0x3

#define ECL_AVC_ME_COST_PRECISION_QPEL_INTEL                 0x0
#define ECL_AVC_ME_COST_PRECISION_HPEL_INTEL                 0x1
#define ECL_AVC_ME_COST_PRECISION_PEL_INTEL                  0x2
#define ECL_AVC_ME_COST_PRECISION_DPEL_INTEL                 0x3

#define ECL_AVC_ME_BIDIR_WEIGHT_QUARTER_INTEL                0x10
#define ECL_AVC_ME_BIDIR_WEIGHT_THIRD_INTEL                  0x15
#define ECL_AVC_ME_BIDIR_WEIGHT_HALF_INTEL                   0x20
#define ECL_AVC_ME_BIDIR_WEIGHT_TWO_THIRD_INTEL              0x2B
#define ECL_AVC_ME_BIDIR_WEIGHT_THREE_QUARTER_INTEL          0x30

#define ECL_AVC_ME_BORDER_REACHED_LEFT_INTEL                 0x0
#define ECL_AVC_ME_BORDER_REACHED_RIGHT_INTEL                0x2
#define ECL_AVC_ME_BORDER_REACHED_TOP_INTEL                  0x4
#define ECL_AVC_ME_BORDER_REACHED_BOTTOM_INTEL               0x8

#define ECL_AVC_ME_SKIP_BLOCK_PARTITION_16x16_INTEL          0x0
#define ECL_AVC_ME_SKIP_BLOCK_PARTITION_8x8_INTEL            0x4000

#define ECL_AVC_ME_SKIP_BLOCK_16x16_FORWARD_ENABLE_INTEL     ( 0x1 << 24 )
#define ECL_AVC_ME_SKIP_BLOCK_16x16_BACKWARD_ENABLE_INTEL    ( 0x2 << 24 )
#define ECL_AVC_ME_SKIP_BLOCK_16x16_DUAL_ENABLE_INTEL        ( 0x3 << 24 )
#define ECL_AVC_ME_SKIP_BLOCK_8x8_FORWARD_ENABLE_INTEL       ( 0x55 << 24 )
#define ECL_AVC_ME_SKIP_BLOCK_8x8_BACKWARD_ENABLE_INTEL      ( 0xAA << 24 )
#define ECL_AVC_ME_SKIP_BLOCK_8x8_DUAL_ENABLE_INTEL          ( 0xFF << 24 )
#define ECL_AVC_ME_SKIP_BLOCK_8x8_0_FORWARD_ENABLE_INTEL     ( 0x1 << 24 )
#define ECL_AVC_ME_SKIP_BLOCK_8x8_0_BACKWARD_ENABLE_INTEL    ( 0x2 << 24 )
#define ECL_AVC_ME_SKIP_BLOCK_8x8_1_FORWARD_ENABLE_INTEL     ( 0x1 << 26 )
#define ECL_AVC_ME_SKIP_BLOCK_8x8_1_BACKWARD_ENABLE_INTEL    ( 0x2 << 26 )
#define ECL_AVC_ME_SKIP_BLOCK_8x8_2_FORWARD_ENABLE_INTEL     ( 0x1 << 28 )
#define ECL_AVC_ME_SKIP_BLOCK_8x8_2_BACKWARD_ENABLE_INTEL    ( 0x2 << 28 )
#define ECL_AVC_ME_SKIP_BLOCK_8x8_3_FORWARD_ENABLE_INTEL     ( 0x1 << 30 )
#define ECL_AVC_ME_SKIP_BLOCK_8x8_3_BACKWARD_ENABLE_INTEL    ( 0x2 << 30 )

#define ECL_AVC_ME_BLOCK_BASED_SKIP_4x4_INTEL                0x00
#define ECL_AVC_ME_BLOCK_BASED_SKIP_8x8_INTEL                0x80

#define ECL_AVC_ME_INTRA_16x16_INTEL                         0x0
#define ECL_AVC_ME_INTRA_8x8_INTEL                           0x1
#define ECL_AVC_ME_INTRA_4x4_INTEL                           0x2

#define ECL_AVC_ME_INTRA_LUMA_PARTITION_MASK_16x16_INTEL     0x6
#define ECL_AVC_ME_INTRA_LUMA_PARTITION_MASK_8x8_INTEL       0x5
#define ECL_AVC_ME_INTRA_LUMA_PARTITION_MASK_4x4_INTEL       0x3

#define ECL_AVC_ME_INTRA_NEIGHBOR_LEFT_MASK_ENABLE_INTEL         0x60
#define ECL_AVC_ME_INTRA_NEIGHBOR_UPPER_MASK_ENABLE_INTEL        0x10
#define ECL_AVC_ME_INTRA_NEIGHBOR_UPPER_RIGHT_MASK_ENABLE_INTEL  0x8
#define ECL_AVC_ME_INTRA_NEIGHBOR_UPPER_LEFT_MASK_ENABLE_INTEL   0x4

#define ECL_AVC_ME_LUMA_PREDICTOR_MODE_VERTICAL_INTEL            0x0
#define ECL_AVC_ME_LUMA_PREDICTOR_MODE_HORIZONTAL_INTEL          0x1
#define ECL_AVC_ME_LUMA_PREDICTOR_MODE_DC_INTEL                  0x2
#define ECL_AVC_ME_LUMA_PREDICTOR_MODE_DIAGONAL_DOWN_LEFT_INTEL  0x3
#define ECL_AVC_ME_LUMA_PREDICTOR_MODE_DIAGONAL_DOWN_RIGHT_INTEL 0x4
#define ECL_AVC_ME_LUMA_PREDICTOR_MODE_PLANE_INTEL               0x4
#define ECL_AVC_ME_LUMA_PREDICTOR_MODE_VERTICAL_RIGHT_INTEL      0x5
#define ECL_AVC_ME_LUMA_PREDICTOR_MODE_HORIZONTAL_DOWN_INTEL     0x6
#define ECL_AVC_ME_LUMA_PREDICTOR_MODE_VERTICAL_LEFT_INTEL       0x7
#define ECL_AVC_ME_LUMA_PREDICTOR_MODE_HORIZONTAL_UP_INTEL       0x8
#define ECL_AVC_ME_CHROMA_PREDICTOR_MODE_DC_INTEL                0x0
#define ECL_AVC_ME_CHROMA_PREDICTOR_MODE_HORIZONTAL_INTEL        0x1
#define ECL_AVC_ME_CHROMA_PREDICTOR_MODE_VERTICAL_INTEL          0x2
#define ECL_AVC_ME_CHROMA_PREDICTOR_MODE_PLANE_INTEL             0x3

#define ECL_AVC_ME_FRAME_FORWARD_INTEL                       0x1
#define ECL_AVC_ME_FRAME_BACKWARD_INTEL                      0x2
#define ECL_AVC_ME_FRAME_DUAL_INTEL                          0x3

#define ECL_AVC_ME_SLICE_TYPE_PRED_INTEL                     0x0
#define ECL_AVC_ME_SLICE_TYPE_BPRED_INTEL                    0x1
#define ECL_AVC_ME_SLICE_TYPE_INTRA_INTEL                    0x2

#define ECL_AVC_ME_INTERLACED_SCAN_TOP_FIELD_INTEL           0x0
#define ECL_AVC_ME_INTERLACED_SCAN_BOTTOM_FIELD_INTEL        0x1

#ifdef __cplusplus
}
#endif

#endif /* __ELCORE_ECL_EXT_INTEL_H */
