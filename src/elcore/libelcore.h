// Copyright 2021 RnD Center "ELVEES", JSC

#ifndef _LIBELCORE_H
#define _LIBELCORE_H_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

typedef std::map<std::string, uint32_t> KernelMap;

class ELFParser;

std::shared_ptr<ELFParser> CreateELFParser(const unsigned char *binary, size_t binary_size);

class ElcoreELF {
 public:
    static std::shared_ptr<ElcoreELF> CreateELF(std::shared_ptr<ELFParser> elf_parser);

    const uint32_t GetEntryPoint() const { return m_entry_point; }
    const uint32_t GetBinarySize() const { return m_binary.size(); }
    const uint8_t *GetBinaryData() const { return m_binary.data(); }
    const KernelMap &GetKernels() const { return m_kernels; }
    const uint32_t GetKernelVirtualAddress(const char *kernel_name) {
        auto kernel_it = m_kernels.find(kernel_name);
        if (kernel_it == m_kernels.end())
            throw std::runtime_error("Failed to find kernel by name");
        return m_kernels[kernel_name];
    }
    ~ElcoreELF() {}

 private:
    std::vector<uint8_t> m_binary;
    uint32_t m_entry_point;
    KernelMap m_kernels;
    ElcoreELF(std::shared_ptr<ELFParser> elf_parser);
};

struct ElcoreSharedSection {
    int mapper_fd;
    size_t size;
    uint64_t vaddr;
};

class ElcoreJobDesc;

class ElcoreJob {
 public:
    static std::shared_ptr<ElcoreJob> CreateElcoreJob(
                    std::shared_ptr<ELFParser> elf_parser,
                    std::shared_ptr<ElcoreELF> elf, int elcore_fd,
                    size_t stack_size);
    static std::shared_ptr<ElcoreJob> CreateElcoreJobWithSharedSections(
                    std::shared_ptr<ELFParser> elf_parser,
                    std::shared_ptr<ElcoreELF> elf, int elcore_fd,
                    size_t stack_size, std::vector<ElcoreSharedSection> sections);
    ~ElcoreJob() {}

    const size_t GetStackSize() const;
    const int GetFD() const;

 private:
    ElcoreJob(std::shared_ptr<ELFParser> elf_parser, std::shared_ptr<ElcoreELF> elf,
              int elcore_fd, size_t stack_size);
    ElcoreJob(std::shared_ptr<ELFParser> elf_parser, std::shared_ptr<ElcoreELF> elf,
              int elcore_fd, size_t stack_size, std::vector<ElcoreSharedSection> sections);
    std::shared_ptr<ElcoreJobDesc> m_elcore_job_desc;
};

int ElcoreJobInstancePoll(int job_instance_fd, int elcore_fd);

#endif  // _LIBELCORE_H_
