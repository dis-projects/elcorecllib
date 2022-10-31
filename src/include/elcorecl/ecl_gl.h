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

#ifndef __ELCORE_ECL_GL_H
#define __ELCORE_ECL_GL_H

#include <elcorecl/ecl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef ecl_uint     ecl_gl_object_type;
typedef ecl_uint     ecl_gl_texture_info;
typedef ecl_uint     ecl_gl_platform_info;
typedef struct __GLsync *ecl_GLsync;

/* ecl_gl_object_type = 0x2000 - 0x200F enum values are currently taken           */
#define ECL_GL_OBJECT_BUFFER                     0x2000
#define ECL_GL_OBJECT_TEXTURE2D                  0x2001
#define ECL_GL_OBJECT_TEXTURE3D                  0x2002
#define ECL_GL_OBJECT_RENDERBUFFER               0x2003
#ifdef ECL_VERSION_1_2
#define ECL_GL_OBJECT_TEXTURE2D_ARRAY            0x200E
#define ECL_GL_OBJECT_TEXTURE1D                  0x200F
#define ECL_GL_OBJECT_TEXTURE1D_ARRAY            0x2010
#define ECL_GL_OBJECT_TEXTURE_BUFFER             0x2011
#endif

/* ecl_gl_texture_info           */
#define ECL_GL_TEXTURE_TARGET                    0x2004
#define ECL_GL_MIPMAP_LEVEL                      0x2005
#ifdef ECL_VERSION_1_2
#define ECL_GL_NUM_SAMPLES                       0x2012
#endif


extern ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateFromGLBuffer(ecl_context     /* context */,
                     ecl_mem_flags   /* flags */,
                     ecl_GLuint      /* bufobj */,
                     int *          /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

#ifdef ECL_VERSION_1_2

extern ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateFromGLTexture(ecl_context      /* context */,
                      ecl_mem_flags    /* flags */,
                      ecl_GLenum       /* target */,
                      ecl_GLint        /* miplevel */,
                      ecl_GLuint       /* texture */,
                      ecl_int *        /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_2;

#endif

extern ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateFromGLRenderbuffer(ecl_context   /* context */,
                           ecl_mem_flags /* flags */,
                           ecl_GLuint    /* renderbuffer */,
                           ecl_int *     /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetGLObjectInfo(ecl_mem                /* memobj */,
                  ecl_gl_object_type *   /* gl_object_type */,
                  ecl_GLuint *           /* gl_object_name */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetGLTextureInfo(ecl_mem               /* memobj */,
                   ecl_gl_texture_info   /* param_name */,
                   size_t               /* param_value_size */,
                   void *               /* param_value */,
                   size_t *             /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueAcquireGLObjects(ecl_command_queue      /* command_queue */,
                          ecl_uint               /* num_objects */,
                          const ecl_mem *        /* mem_objects */,
                          ecl_uint               /* num_events_in_wait_list */,
                          const ecl_event *      /* event_wait_list */,
                          ecl_event *            /* event */) ECL_API_SUFFIX__VERSION_1_0;

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueReleaseGLObjects(ecl_command_queue      /* command_queue */,
                          ecl_uint               /* num_objects */,
                          const ecl_mem *        /* mem_objects */,
                          ecl_uint               /* num_events_in_wait_list */,
                          const ecl_event *      /* event_wait_list */,
                          ecl_event *            /* event */) ECL_API_SUFFIX__VERSION_1_0;


/* Deprecated OpenCL 1.1 APIs */
extern ECL_API_ENTRY ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED ecl_mem ECL_API_CALL
eclCreateFromGLTexture2D(ecl_context      /* context */,
                        ecl_mem_flags    /* flags */,
                        ecl_GLenum       /* target */,
                        ecl_GLint        /* miplevel */,
                        ecl_GLuint       /* texture */,
                        ecl_int *        /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern ECL_API_ENTRY ECL_EXT_PREFIX__VERSION_1_1_DEPRECATED ecl_mem ECL_API_CALL
eclCreateFromGLTexture3D(ecl_context      /* context */,
                        ecl_mem_flags    /* flags */,
                        ecl_GLenum       /* target */,
                        ecl_GLint        /* miplevel */,
                        ecl_GLuint       /* texture */,
                        ecl_int *        /* errcode_ret */) ECL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

/* ecl_khr_gl_sharing extension  */

#define ecl_khr_gl_sharing 1

typedef ecl_uint     ecl_gl_context_info;

/* Additional Error Codes  */
#define ECL_INVALID_GL_SHAREGROUP_REFERENCE_KHR  -1000

/* ecl_gl_context_info  */
#define ECL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR    0x2006
#define ECL_DEVICES_FOR_GL_CONTEXT_KHR           0x2007

/* Additional ecl_context_properties  */
#define ECL_GL_CONTEXT_KHR                       0x2008
#define ECL_EGL_DISPLAY_KHR                      0x2009
#define ECL_GLX_DISPLAY_KHR                      0x200A
#define ECL_WGL_HDC_KHR                          0x200B
#define ECL_CGL_SHAREGROUP_KHR                   0x200C

extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetGLContextInfoKHR(const ecl_context_properties * /* properties */,
                      ecl_gl_context_info            /* param_name */,
                      size_t                        /* param_value_size */,
                      void *                        /* param_value */,
                      size_t *                      /* param_value_size_ret */) ECL_API_SUFFIX__VERSION_1_0;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclGetGLContextInfoKHR_fn)(
    const ecl_context_properties * properties,
    ecl_gl_context_info            param_name,
    size_t                        param_value_size,
    void *                        param_value,
    size_t *                      param_value_size_ret);

#ifdef __cplusplus
}
#endif

#endif  /* __ELCORE_ECL_GL_H */
