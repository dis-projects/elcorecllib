// Copyright 2018-2019 RnD Center "ELVEES", JSC
/*******************************************************************************
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
 ******************************************************************************/

#ifndef __ELCORE_ECL_EGL_H
#define __ELCORE_ECL_EGL_H

#include <elcorecl/ecl.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Command type for events created with eclEnqueueAcquireEGLObjectsKHR */
#define ECL_COMMAND_EGL_FENCE_SYNC_OBJECT_KHR  0x202F
#define ECL_COMMAND_ACQUIRE_EGL_OBJECTS_KHR    0x202D
#define ECL_COMMAND_RELEASE_EGL_OBJECTS_KHR    0x202E

/* Error type for eclCreateFromEGLImageKHR */
#define ECL_INVALID_EGL_OBJECT_KHR             -1093
#define ECL_EGL_RESOURCE_NOT_ACQUIRED_KHR      -1092

/* ECLeglImageKHR is an opaque handle to an EGLImage */
typedef void* ECLeglImageKHR;

/* ECLeglDisplayKHR is an opaque handle to an EGLDisplay */
typedef void* ECLeglDisplayKHR;

/* ECLeglSyncKHR is an opaque handle to an EGLSync object */
typedef void* ECLeglSyncKHR;

/* properties passed to eclCreateFromEGLImageKHR */
typedef intptr_t ecl_egl_image_properties_khr;


#define ecl_khr_egl_image 1

extern ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateFromEGLImageKHR(ecl_context                  /* context */,
                        ECLeglDisplayKHR             /* egldisplay */,
                        ECLeglImageKHR               /* eglimage */,
                        ecl_mem_flags                /* flags */,
                        const ecl_egl_image_properties_khr * /* properties */,
                        ecl_int *                    /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

typedef ECL_API_ENTRY ecl_mem (ECL_API_CALL *eclCreateFromEGLImageKHR_fn)(
  ecl_context                  context,
  ECLeglDisplayKHR             egldisplay,
  ECLeglImageKHR               eglimage,
  ecl_mem_flags                flags,
  const ecl_egl_image_properties_khr * properties,
  ecl_int *                    errcode_ret);


extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueAcquireEGLObjectsKHR(ecl_command_queue /* command_queue */,
                              ecl_uint          /* num_objects */,
                              const ecl_mem *   /* mem_objects */,
                              ecl_uint          /* num_events_in_wait_list */,
                              const ecl_event * /* event_wait_list */,
                              ecl_event *       /* event */) ECL_API_SUFFIX__VERSION_1_0;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclEnqueueAcquireEGLObjectsKHR_fn)(
  ecl_command_queue command_queue,
  ecl_uint          num_objects,
  const ecl_mem *   mem_objects,
  ecl_uint          num_events_in_wait_list,
  const ecl_event * event_wait_list,
  ecl_event *       event);


extern ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueReleaseEGLObjectsKHR(ecl_command_queue /* command_queue */,
                              ecl_uint          /* num_objects */,
                              const ecl_mem *   /* mem_objects */,
                              ecl_uint          /* num_events_in_wait_list */,
                              const ecl_event * /* event_wait_list */,
                              ecl_event *       /* event */) ECL_API_SUFFIX__VERSION_1_0;

typedef ECL_API_ENTRY ecl_int (ECL_API_CALL *eclEnqueueReleaseEGLObjectsKHR_fn)(
  ecl_command_queue command_queue,
  ecl_uint          num_objects,
  const ecl_mem *   mem_objects,
  ecl_uint          num_events_in_wait_list,
  const ecl_event * event_wait_list,
  ecl_event *       event);


#define ecl_khr_egl_event 1

extern ECL_API_ENTRY ecl_event ECL_API_CALL
eclCreateEventFromEGLSyncKHR(ecl_context      /* context */,
                            ECLeglSyncKHR    /* sync */,
                            ECLeglDisplayKHR /* display */,
                            ecl_int *        /* errcode_ret */) ECL_API_SUFFIX__VERSION_1_0;

typedef ECL_API_ENTRY ecl_event (ECL_API_CALL *eclCreateEventFromEGLSyncKHR_fn)(
  ecl_context      context,
  ECLeglSyncKHR    sync,
  ECLeglDisplayKHR display,
  ecl_int *        errcode_ret);

#ifdef __cplusplus
}
#endif

#endif /* __ELCORE_ECL_EGL_H */
