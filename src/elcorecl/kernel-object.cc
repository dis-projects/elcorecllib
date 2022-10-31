// Copyright 2018 RnD Center "ELVEES", JSC

#include <cassert>
#include <memory>
#include <mutex>
#include <vector>
#include <utility>

#include "elcore-cl.h"

extern "C" ECL_API_ENTRY ecl_kernel ECL_API_CALL
eclCreateKernel(ecl_program program, const char* kernel_name,
                ecl_int* errcode_ret) ECL_API_SUFFIX__VERSION_1_0 {
    if (kernel_name == nullptr) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_VALUE;
        return nullptr;
    }

    if (program == nullptr) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_PROGRAM;
        return nullptr;
    }

    if (program->m_build_status != ECL_BUILD_SUCCESS) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_PROGRAM_EXECUTABLE;
        return nullptr;
    }

    auto& kernels = program->m_elf->GetKernels();
    auto kernel_it = kernels.find(kernel_name);
    if (kernel_it == kernels.end()) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_KERNEL_NAME;
        return nullptr;
    }
    boost::intrusive_ptr<_ecl_kernel> kernel =
        new _ecl_kernel(program, kernel_name, kernel_it->second);
    kernel->add_ref();
    if (errcode_ret) *errcode_ret = ECL_SUCCESS;
    return kernel.get();
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclCreateKernelsInProgram(ecl_program program, ecl_uint num_kernels, ecl_kernel* kernels,
                          ecl_uint* num_kernels_ret) ECL_API_SUFFIX__VERSION_1_0 {
    if (program == nullptr || program->m_devices.size() == 0) return ECL_INVALID_PROGRAM;

    if (program->m_build_status != ECL_BUILD_SUCCESS) return ECL_INVALID_PROGRAM_EXECUTABLE;

    if ((kernels != nullptr && num_kernels == 0) || (kernels == nullptr && num_kernels != 0) ||
        (kernels && num_kernels < program->m_elf->GetKernels().size()))
        return ECL_INVALID_VALUE;

    for (ecl_uint i = 0; i < num_kernels; i++) kernels[i] = nullptr;

    if (num_kernels > program->m_elf->GetKernels().size())
        num_kernels = program->m_elf->GetKernels().size();

    ecl_int error_ret;
    if (num_kernels > 0 && kernels != nullptr) {
        auto kernel_it = program->m_elf->GetKernels().cbegin();
        for (ecl_uint i = 0; i < num_kernels; ++i, ++kernel_it) {
            kernels[i] = eclCreateKernel(program, kernel_it->first.c_str(), &error_ret);

            assert(error_ret != ECL_INVALID_KERNEL_NAME);
            assert(error_ret != ECL_INVALID_VALUE);
            if (error_ret != ECL_SUCCESS) {
                for (; i > 0; i--) eclReleaseKernel(kernels[i - 1]);
                return error_ret;
            }
        }
    }

    if (num_kernels_ret) *num_kernels_ret = program->m_elf->GetKernels().size();
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_kernel ECL_API_CALL
eclCloneKernel(ecl_kernel source_kernel, ecl_int* errcode_ret) ECL_API_SUFFIX__VERSION_2_1 {
    if (source_kernel == nullptr) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_KERNEL;
        return nullptr;
    }

    boost::intrusive_ptr<_ecl_kernel> kernel = new _ecl_kernel(source_kernel);
    if (errcode_ret) *errcode_ret = ECL_SUCCESS;
    kernel->add_ref();
    return kernel.get();
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainKernel(ecl_kernel kernel) ECL_API_SUFFIX__VERSION_1_0 {
    if (kernel == nullptr) return ECL_INVALID_KERNEL;
    kernel->add_ref();
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseKernel(ecl_kernel kernel) ECL_API_SUFFIX__VERSION_1_0 {
    if (kernel == nullptr) return ECL_INVALID_KERNEL;
    kernel->release();
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelArg(ecl_kernel kernel, ecl_uint arg_index, size_t arg_size,
                const void* arg_value) ECL_API_SUFFIX__VERSION_1_0 {
    if (kernel == nullptr) return ECL_INVALID_KERNEL;

    if (arg_index >= ELCORE50_MAX_JOB_ARGS) return ECL_INVALID_ARG_INDEX;

    if (arg_value == nullptr) return ECL_INVALID_ARG_VALUE;

    std::unique_lock<std::mutex> lock(kernel->m_kernel_guard);

    const uint8_t* data_ptr = reinterpret_cast<const uint8_t*>(arg_value);
    kernel->m_args[arg_index] = std::move(std::pair<ecl_uint, std::vector<uint8_t>>(
        ECL_KERNEL_ARG_TYPE_BASIC, std::vector<uint8_t>(data_ptr, data_ptr + arg_size)));
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelArgELcoreMem(ecl_kernel kernel, ecl_uint arg_index,
                         const ecl_mem arg_value) ECL_API_SUFFIX__VERSION_1_0 {
    if (kernel == nullptr) return ECL_INVALID_KERNEL;

    if (arg_index >= ELCORE50_MAX_JOB_ARGS) return ECL_INVALID_ARG_INDEX;

    if (arg_value == nullptr) return ECL_INVALID_ARG_VALUE;

    std::unique_lock<std::mutex> lock(kernel->m_kernel_guard);

    const uint8_t* data_ptr = reinterpret_cast<const uint8_t*>(&arg_value);
    kernel->m_args[arg_index] = std::move(std::pair<ecl_uint, std::vector<uint8_t>>(
        ECL_KERNEL_ARG_TYPE_GLOBAL, std::vector<uint8_t>(data_ptr, data_ptr + sizeof(ecl_mem))));
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelArgELcoreMemNonCached(ecl_kernel kernel, ecl_uint arg_index,
                                  const ecl_mem arg_value) ECL_API_SUFFIX__VERSION_1_0 {
    if (kernel == nullptr) return ECL_INVALID_KERNEL;

    if (arg_index >= ELCORE50_MAX_JOB_ARGS) return ECL_INVALID_ARG_INDEX;

    if (arg_value == nullptr) return ECL_INVALID_ARG_VALUE;

    std::unique_lock<std::mutex> lock(kernel->m_kernel_guard);

    const uint8_t* data_ptr = reinterpret_cast<const uint8_t*>(&arg_value);
    kernel->m_args[arg_index] = std::move(std::pair<ecl_uint, std::vector<uint8_t>>(
        ECL_KERNEL_ARG_TYPE_NC_GLOBAL, std::vector<uint8_t>(data_ptr, data_ptr + sizeof(ecl_mem))));
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelArgELcoreLocalMem(ecl_kernel kernel, ecl_uint arg_index,
                              const ecl_uint local_mem_size) ECL_API_SUFFIX__VERSION_1_0 {
    if (kernel == nullptr) return ECL_INVALID_KERNEL;

    if (arg_index >= ELCORE50_MAX_JOB_ARGS) return ECL_INVALID_ARG_INDEX;

    if (local_mem_size == 0) return ECL_INVALID_ARG_SIZE;

    std::unique_lock<std::mutex> lock(kernel->m_kernel_guard);

    const uint8_t* data_ptr = reinterpret_cast<const uint8_t*>(&local_mem_size);
    kernel->m_args[arg_index] = std::move(std::pair<ecl_uint, std::vector<uint8_t>>(
        ECL_KERNEL_ARG_TYPE_LOCAL,
        std::vector<uint8_t>(data_ptr, data_ptr + sizeof(local_mem_size))));
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetKernelArgDMAMem(ecl_kernel kernel, ecl_uint arg_index,
                         const ecl_mem arg_value) ECL_API_SUFFIX__VERSION_1_0 {
    if (kernel == nullptr) return ECL_INVALID_KERNEL;

    if (arg_index >= ELCORE50_MAX_JOB_ARGS) return ECL_INVALID_ARG_INDEX;

    if (arg_value == nullptr) return ECL_INVALID_ARG_VALUE;

    std::unique_lock<std::mutex> lock(kernel->m_kernel_guard);

    const uint8_t* data_ptr = reinterpret_cast<const uint8_t*>(&arg_value);
    kernel->m_args[arg_index] = std::move(std::pair<ecl_uint, std::vector<uint8_t>>(
        ECL_KERNEL_ARG_TYPE_DMA, std::vector<uint8_t>(data_ptr, data_ptr + sizeof(ecl_mem))));
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetKernelInfo(ecl_kernel kernel, ecl_kernel_info param_name, size_t param_value_size,
                 void* param_value, size_t* param_value_size_ret) ECL_API_SUFFIX__VERSION_1_0 {
    if (kernel == nullptr) return ECL_INVALID_KERNEL;

    switch (param_name) {
        case ECL_KERNEL_FUNCTION_NAME:
            ECL_RETURN_GETINFO_STR(kernel->m_kernel_name.c_str());
        case ECL_KERNEL_REFERENCE_COUNT:
            ECL_RETURN_GETINFO(ecl_uint, kernel->ref_count());
        case ECL_KERNEL_CONTEXT:
            ECL_RETURN_GETINFO(ecl_context, kernel->m_program->m_context.get());
        case ECL_KERNEL_PROGRAM:
            ECL_RETURN_GETINFO(ecl_program, kernel->m_program.get());
        case ECL_KERNEL_ATTRIBUTES:
            ECL_RETURN_GETINFO_STR("");
    }
    return ECL_INVALID_VALUE;
}
