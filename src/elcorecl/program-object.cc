// Copyright 2018 RnD Center "ELVEES", JSC

#include <ios>
#include <istream>
#include <memory>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

#include <stdlib.h>
#include <unistd.h>

#include "elcore-cl.h"

#define E50_PAGE_MASK (4 * 1024 - 1)

static void SetBinaryStatuses(ecl_int* const binary_status, const ecl_uint num_devices,
                              const ecl_int status) {
    if (binary_status != nullptr) {
        for (ecl_uint i = 0; i < num_devices; ++i) {
            binary_status[i] = status;
        }
    }
}

static ecl_program
CreateProgramWithBinary(ecl_context context, ecl_uint num_devices,
                        const ecl_device_id* device_list, const size_t* lengths,
                        const unsigned char** binaries, ecl_int* binary_status,
                        ecl_uint num_shared_sections, const ecl_shared_section *shared_sections,
                        ecl_int* errcode_ret) {
    ecl_int result;
    if (context == nullptr) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_CONTEXT;
        return nullptr;
    }

    if (device_list == nullptr || lengths == nullptr) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_VALUE;
        return nullptr;
    }

    if (num_devices == 0) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_VALUE;
        return nullptr;
    }

    for (size_t i = 0; i < num_devices; ++i) {
        if (lengths[i] == 0 || binaries[i] == nullptr || binaries[i] != binaries[0] ||
            lengths[i] != lengths[0]) {
            SetBinaryStatuses(binary_status, num_devices, ECL_INVALID_VALUE);
            if (errcode_ret) *errcode_ret = ECL_INVALID_VALUE;
            return nullptr;
        }
    }

    // check for duplicates in device_list[].
    for (std::size_t i = 0; i < context->m_device_ids.size(); i++) {
        int count = 0;
        for (size_t j = 0; j < num_devices; j++) {
            count += context->m_device_ids[i] == device_list[j];
        }
        // duplicate devices
        if (count > 1) {
            if (errcode_ret) *errcode_ret = ECL_INVALID_DEVICE;
            return nullptr;
        }
    }

    // check for invalid devices in device_list[].
    for (size_t i = 0; i < num_devices; i++) {
        bool found = 0;
        for (std::size_t j = 0; j < context->m_device_ids.size(); j++) {
            found |= context->m_device_ids[j] == device_list[i];
        }
        if (!found) {
            if (errcode_ret) *errcode_ret = ECL_INVALID_DEVICE;
            return nullptr;
        }
    }

    boost::intrusive_ptr<_ecl_program> program(new _ecl_program(context, num_devices,
                                                                device_list));

    std::vector<ElcoreSharedSection> elcore_shared_sections_list;
    // check for invalid shared_sections in shared_sections
    for (size_t i = 0; i < num_shared_sections; i++) {
        ecl_mem mem = shared_sections[i].mem;
        ecl_ulong paddr = mem->m_mem_info.p;
        ecl_ulong vaddr = shared_sections[i].virtual_addr;

        // check if memory object is created
        if (!mem->m_mem_info.mapper_fd) {
            if (errcode_ret) *errcode_ret = ECL_INVALID_ARG_VALUE;
            return nullptr;
        }

        if ((vaddr & E50_PAGE_MASK) != (paddr & E50_PAGE_MASK)) {
            if (errcode_ret) *errcode_ret = ECL_INVALID_ARG_VALUE;
            return nullptr;
        }
        ElcoreSharedSection elcore_shared_section;
        elcore_shared_section.mapper_fd = mem->m_mem_info.mapper_fd;
        elcore_shared_section.size = mem->m_size;
        elcore_shared_section.vaddr = vaddr;
        elcore_shared_sections_list.push_back(elcore_shared_section);
        program->m_shared_sections.push_back(mem);
    }

    std::shared_ptr<ELFParser> parser;
    try {
        parser = CreateELFParser(binaries[0], lengths[0]);
    } catch(const std::runtime_error& e) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_BINARY;
        return nullptr;
    }

    program->m_elf = ElcoreELF::CreateELF(parser);
    std::vector<std::shared_ptr<_ecl_device>> &devices = context->m_devices;
    try {
        for (ecl_uint dev = 0; dev < num_devices; dev++) {
            std::shared_ptr<ElcoreJob> job;
            if (num_shared_sections)
                job = ElcoreJob::CreateElcoreJobWithSharedSections(
                                                    parser, program->m_elf, devices[dev]->m_fd,
                                                 program->get_stack_size(),
                                                 elcore_shared_sections_list);
            else
                job = ElcoreJob::CreateElcoreJob(parser, program->m_elf, devices[dev]->m_fd,
                                                 program->get_stack_size());
            if (!job) return nullptr;
            program->m_jobs.push_back(job);
        }
    } catch(const std::runtime_error& e) {
        if (!strcmp(e.what(), "possible ELCORE_HUGEPAGE_ENABLE values: 0, 1"))
            result = ECL_INVALID_VALUE;
        else if (!strcmp(e.what(), "ELCORE50_IOC_CREATE_JOB failed"))
            result = ECL_DEVICE_NOT_AVAILABLE;
        else
            result = ECL_OUT_OF_HOST_MEMORY;
        if (errcode_ret) *errcode_ret = result;
        return nullptr;
    }

    try {
        for (ecl_uint dev = 0; dev < num_devices; dev++) {
            program->m_job_instance_templates[dev].entry_point_virtual_address =
                program->m_elf->GetEntryPoint();
            program->m_job_instance_templates[dev].launcher_virtual_address =
                program->m_elf->GetKernelVirtualAddress("_elcore_job_launcher");
            strcpy(program->m_job_instance_templates[dev].name, "_elcore_job_launcher");
            program->m_job_instance_templates[dev].argc = 0;
            program->m_job_instance_templates[dev].job_fd = program->m_jobs[dev]->GetFD();
            program->m_job_instance_templates[dev].debug_enable = 0;
            if (devices[dev]->enqueue_job(&program->m_job_instance_templates[dev]) != 0) {
                if (errcode_ret) *errcode_ret = ECL_DEVICE_NOT_AVAILABLE;
                return nullptr;
            }
        }
    } catch(const std::runtime_error& e) {
        // _elcore_job_launcher was not found
        if (errcode_ret) *errcode_ret = ECL_INVALID_BINARY;
        return nullptr;
    }

    for (ecl_uint dev = 0; dev < num_devices; dev++) {
        int job_instance_fd = program->m_job_instance_templates[dev].job_instance_fd;
        try {
            int status = ElcoreJobInstancePoll(job_instance_fd, devices[dev]->m_fd);
            if (status != ELCORE50_JOB_STATUS_SUCCESS) {
                if (errcode_ret) *errcode_ret = ECL_INVALID_BINARY;
                return nullptr;
            }
        } catch(const std::runtime_error& e) {
            if (errcode_ret) *errcode_ret = ECL_DEVICE_NOT_AVAILABLE;
            return nullptr;
        }
        if (devices[dev]->close_job(job_instance_fd) != 0) {
            if (errcode_ret) *errcode_ret = ECL_DEVICE_NOT_AVAILABLE;
            return nullptr;
        }
    }
    program->m_build_status = ECL_BUILD_SUCCESS;
    if (errcode_ret) *errcode_ret = ECL_SUCCESS;
    program->add_ref();
    return program.get();
}

extern "C" ECL_API_ENTRY ecl_program ECL_API_CALL
eclCreateProgramWithBinary(ecl_context context, ecl_uint num_devices,
                           const ecl_device_id* device_list, const size_t* lengths,
                           const unsigned char** binaries, ecl_int* binary_status,
                           ecl_int* errcode_ret) ECL_API_SUFFIX__VERSION_1_0 {
    return CreateProgramWithBinary(context, num_devices, device_list, lengths, binaries,
                                   binary_status, 0, NULL, errcode_ret);
}

extern "C" ECL_API_ENTRY ecl_program ECL_API_CALL
eclCreateProgramWithSharedSections(ecl_context context, ecl_uint num_devices,
                           const ecl_device_id* device_list, const size_t* lengths,
                           const unsigned char** binaries, ecl_int* binary_status,
                           ecl_uint num_shared_sections, const ecl_shared_section *shared_sections,
                           ecl_int* errcode_ret) ECL_API_SUFFIX__VERSION_1_0 {
    return CreateProgramWithBinary(context, num_devices, device_list, lengths, binaries,
                                   binary_status, num_shared_sections, shared_sections,
                                   errcode_ret);
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainProgram(ecl_program program) ECL_API_SUFFIX__VERSION_1_0 {
    if (!program) return ECL_INVALID_PROGRAM;
    program->add_ref();
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseProgram(ecl_program program) ECL_API_SUFFIX__VERSION_1_0 {
    if (!program) return ECL_INVALID_PROGRAM;
    program->release();
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetProgramReleaseCallback(ecl_program program,
                             void(ECL_CALLBACK* pfn_notify)(ecl_program /* program */,
                                                            void* /* user_data */),
                             void* user_data) ECL_API_SUFFIX__VERSION_2_2 {
    if (program == nullptr) return ECL_INVALID_PROGRAM;
    if (pfn_notify == nullptr) return ECL_INVALID_VALUE;
    program->m_release_callbacks.push_back(
        std::pair<void(ECL_CALLBACK*)(ecl_program, void*), void*>(pfn_notify, user_data));
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetProgramInfo(ecl_program program, ecl_program_info param_name, size_t param_value_size,
                  void* param_value, size_t* param_value_size_ret) ECL_API_SUFFIX__VERSION_1_0 {
    unsigned i;

    if (program == nullptr) return ECL_INVALID_PROGRAM;

    switch (param_name) {
        case ECL_PROGRAM_REFERENCE_COUNT:
            ECL_RETURN_GETINFO(ecl_uint, (ecl_uint)program->ref_count());
        case ECL_PROGRAM_CONTEXT:
            ECL_RETURN_GETINFO(ecl_context, program->m_context.get());
        case ECL_PROGRAM_SOURCE: {
            const char* source = "";
            ECL_RETURN_GETINFO_STR(source);
        }
        case ECL_PROGRAM_BINARY_SIZES: {
            size_t const value_size = sizeof(size_t) * program->m_devices.size();
            ECL_RETURN_GETINFO_INNER(value_size, {
                for (std::size_t i = 0; i < program->m_devices.size(); ++i) {
                    ((size_t*)param_value)[i] = program->m_elf->GetBinarySize();
                }
            });
        }
        case ECL_PROGRAM_BINARIES: {
            const size_t value_size = sizeof(unsigned char*) * program->m_devices.size();
            ECL_RETURN_GETINFO_INNER(value_size, {
                for (std::size_t i = 0; i < program->m_devices.size(); ++i) {
                    ((const unsigned char**)param_value)[i] = program->m_elf->GetBinaryData();
                }
            });
        }
        case ECL_PROGRAM_NUM_DEVICES:
            ECL_RETURN_GETINFO(ecl_uint, program->m_devices.size());
        case ECL_PROGRAM_DEVICES: {
            size_t const value_size = sizeof(ecl_device_id) * program->m_devices.size();
            ECL_RETURN_GETINFO_SIZE(value_size, program->m_devices.data());
        }
        case ECL_PROGRAM_NUM_KERNELS:
            ECL_RETURN_GETINFO(size_t, program->m_elf->GetKernels().size());
        case ECL_PROGRAM_KERNEL_NAMES: {
            size_t size = 0;

            for (auto it = program->m_elf->GetKernels().cbegin();
                     it != program->m_elf->GetKernels().cend(); ++it) {
                size += it->first.size() + 1;  // a semicolon
            }
            size += 1;  // a NULL
            if (param_value_size_ret) *param_value_size_ret = size;
            if (param_value) {
                /* only when param_value is non-NULL */
                if (size > param_value_size) return ECL_INVALID_VALUE;
                /* should not break from the switch clause
                   because of POCL_ABORT_UNIMPLEMENTED */
                std::string out_str;
                for (auto it = program->m_elf->GetKernels().cbegin();
                         it != program->m_elf->GetKernels().cend(); ++it) {
                    out_str += it->first.size() + ";";  // a semicolon
                }
                strcpy(reinterpret_cast<char*>(param_value), out_str.c_str());
            }
            return ECL_SUCCESS;
        }
        case ECL_PROGRAM_STACK_SIZE:
            ECL_RETURN_GETINFO(size_t, program->get_stack_size());
    }
    return ECL_INVALID_VALUE;
}
