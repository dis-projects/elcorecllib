// Copyright 2018 RnD Center "ELVEES", JSC

#include <cstdint>
#include <utility>

#include <unistd.h>

#include "elcore-cl.h"

namespace {
ecl_int ValidateBaseArguments(ecl_context context, ecl_mem_flags flags, size_t size,
                              ecl_char is_dma_buffer) {
    if (size == 0)
        return ECL_INVALID_BUFFER_SIZE;

    if (context == nullptr)
        return ECL_INVALID_CONTEXT;

    if (flags == 0) flags = ECL_MEM_READ_WRITE;
    /* validate flags */
    if (flags &
        (~(ECL_MEM_READ_WRITE | ECL_MEM_WRITE_ONLY | ECL_MEM_READ_ONLY | ECL_MEM_USE_HOST_PTR |
           ECL_MEM_ALLOC_HOST_PTR | ECL_MEM_COPY_HOST_PTR | ECL_MEM_HOST_WRITE_ONLY |
           ECL_MEM_HOST_READ_ONLY | ECL_MEM_HOST_NO_ACCESS | ECL_MEM_SVM_FINE_GRAIN_BUFFER |
           ECL_MEM_SVM_ATOMICS | ECL_MEM_KERNEL_READ_AND_WRITE))) {
        return ECL_INVALID_VALUE;
    }
    if (((flags & ECL_MEM_READ_WRITE) &&
         (flags & ECL_MEM_WRITE_ONLY || flags & ECL_MEM_READ_ONLY)) ||
        ((flags & ECL_MEM_READ_ONLY) && (flags & ECL_MEM_WRITE_ONLY)) ||
        ((flags & ECL_MEM_USE_HOST_PTR) &&
         (flags & ECL_MEM_ALLOC_HOST_PTR || flags & ECL_MEM_COPY_HOST_PTR)) ||
        ((flags & ECL_MEM_HOST_WRITE_ONLY) && (flags & ECL_MEM_HOST_READ_ONLY)) ||
        ((flags & ECL_MEM_HOST_NO_ACCESS) &&
         ((flags & ECL_MEM_HOST_READ_ONLY) || (flags & ECL_MEM_HOST_WRITE_ONLY)))) {
        return ECL_INVALID_VALUE;
    }
    ecl_ulong max_alloc;

    if (is_dma_buffer)
        eclGetDeviceInfo(context->m_device_ids[0], ECL_DEVICE_MAX_DMAMEM_ALLOC_SIZE,
                         sizeof(ecl_ulong), &max_alloc, nullptr);
    else
        eclGetDeviceInfo(context->m_device_ids[0], ECL_DEVICE_MAX_MEM_ALLOC_SIZE,
                         sizeof(ecl_ulong), &max_alloc, nullptr);

    for (std::size_t i = 0; i < context->m_device_ids.size(); ++i) {
        if (size > max_alloc)
            return ECL_INVALID_BUFFER_SIZE;
    }
    return ECL_SUCCESS;
}

ecl_int ValidateArguments(ecl_context context, ecl_mem_flags flags, size_t size,
                          void* host_ptr, ecl_char is_dma_buffer) {
    ecl_int ret = ValidateBaseArguments(context, flags, size, is_dma_buffer);
    if (ret != ECL_SUCCESS) {
        return ret;
    }
    if (host_ptr == nullptr) {
        return ECL_INVALID_HOST_PTR;
    }
    if (!(flags & ECL_MEM_USE_HOST_PTR)) {
        return ECL_INVALID_VALUE;
    }
    const int cacheline_size = 64;
    if ((reinterpret_cast<uintptr_t>(host_ptr) % cacheline_size) != 0 ||
        (size % cacheline_size) != 0) {
        return ECL_INVALID_HOST_PTR;
    }
    return ECL_SUCCESS;
}
}

extern "C" ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateBuffer(ecl_context context, ecl_mem_flags flags, size_t size, void* host_ptr,
                ecl_int* errcode_ret) ECL_API_SUFFIX__VERSION_1_0 {
    ecl_int ret;

    ret = ValidateArguments(context, flags, size, host_ptr, 0);
    if (ret != ECL_SUCCESS) {
        if (errcode_ret) *errcode_ret = ret;
        return nullptr;
    }
    madvise(host_ptr, size, MADV_HUGEPAGE);

    boost::intrusive_ptr<_ecl_mem> mem =
        new _ecl_mem(context, flags, size, host_ptr, ECL_MEM_OBJECT_BUFFER);

    if (errcode_ret != nullptr) *errcode_ret = ECL_SUCCESS;
    mem->add_ref();
    return mem.get();
}

extern "C" ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateBufferFromDmaBuf(ecl_context context, ecl_mem_flags flags, ecl_int fd, size_t size,
                          ecl_int *errcode_ret) ECL_API_SUFFIX__VERSION_1_0 {
    ecl_int ret;

    ret = ValidateBaseArguments(context, flags, size, 0);
    if (ret != ECL_SUCCESS) {
        if (errcode_ret) *errcode_ret = ret;
        return nullptr;
    }
    if (!fd) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_VALUE;
        return nullptr;
    }

    boost::intrusive_ptr<_ecl_mem> mem =
        new _ecl_mem(context, flags | ECL_MEM_USE_DMABUF, size, NULL, ECL_MEM_OBJECT_BUFFER, fd);

    if (errcode_ret != nullptr) *errcode_ret = ECL_SUCCESS;
    mem->add_ref();
    return mem.get();
}

extern "C" ECL_API_ENTRY ecl_mem ECL_API_CALL
eclCreateDMABuffer(ecl_context context, ecl_mem_flags flags, size_t size, void* host_ptr,
                   ecl_int* errcode_ret) ECL_API_SUFFIX__VERSION_1_0 {
    ecl_int ret;

    ret = ValidateArguments(context, flags, size, host_ptr, 1);
    if (ret != ECL_SUCCESS) {
        if (errcode_ret) *errcode_ret = ret;
        return nullptr;
    }
    madvise(host_ptr, size, MADV_HUGEPAGE);

    boost::intrusive_ptr<_ecl_mem> mem =
        new _ecl_mem(context, flags, size, host_ptr, ECL_MEM_OBJECT_BUFFER);

    if (errcode_ret != nullptr) *errcode_ret = ECL_SUCCESS;
    mem->add_ref();
    return mem.get();
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainMemObject(ecl_mem memobj) ECL_API_SUFFIX__VERSION_1_0 {
    if (memobj == nullptr) return ECL_INVALID_MEM_OBJECT;
    memobj->add_ref();
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseMemObject(ecl_mem memobj) ECL_API_SUFFIX__VERSION_1_0 {
    if (memobj == nullptr) return ECL_INVALID_MEM_OBJECT;
    memobj->release();
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetMemObjectInfo(ecl_mem memobj, ecl_mem_info param_name, size_t param_value_size,
                    void* param_value, size_t* param_value_size_ret) ECL_API_SUFFIX__VERSION_1_0 {
    if (memobj == nullptr) {
        return ECL_INVALID_MEM_OBJECT;
    }

    switch (param_name) {
        case ECL_MEM_TYPE:
            ECL_RETURN_GETINFO(ecl_mem_object_type, memobj->m_type);
        case ECL_MEM_FLAGS:
            ECL_RETURN_GETINFO(ecl_mem_flags, memobj->m_flags);
        case ECL_MEM_SIZE:
            ECL_RETURN_GETINFO(size_t, memobj->m_size);
        case ECL_MEM_HOST_PTR:
            if (memobj->m_flags & ECL_MEM_USE_HOST_PTR)
                ECL_RETURN_GETINFO(void*, memobj->m_host_ptr);
            else
                ECL_RETURN_GETINFO(void*, nullptr);
        case ECL_MEM_MAP_COUNT:
            ECL_RETURN_GETINFO(ecl_uint, 0);
        case ECL_MEM_REFERENCE_COUNT:
            ECL_RETURN_GETINFO(ecl_uint, memobj->ref_count());
        case ECL_MEM_CONTEXT:
            ECL_RETURN_GETINFO(ecl_context, memobj->m_context.get());
        case ECL_MEM_ASSOCIATED_MEMOBJECT:
            ECL_RETURN_GETINFO(ecl_mem, nullptr);
        case ECL_MEM_OFFSET:
            ECL_RETURN_GETINFO(size_t, 0);
    }
    return ECL_INVALID_VALUE;
}

#ifdef ECL_VERSION_1_1

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetMemObjectDestructorCallback(ecl_mem memobj, void(ECL_CALLBACK* pfn_notify)(ecl_mem, void*),
                                  void* user_data) ECL_API_SUFFIX__VERSION_1_1 {
    if (memobj == nullptr) return ECL_INVALID_MEM_OBJECT;
    if (pfn_notify == nullptr) return ECL_INVALID_VALUE;
    memobj->m_destructor_callbacks.emplace_back(pfn_notify, user_data);
    return ECL_SUCCESS;
}

#endif
