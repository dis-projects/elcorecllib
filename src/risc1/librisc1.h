// Copyright 2021-2022 RnD Center "ELVEES", JSC

#ifndef _LIBRISC1_H
#define _LIBRISC1_H_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>
#include <iostream>

typedef std::map<std::string, uint32_t> KernelMap;

class RISC1ELFParser;

std::shared_ptr<RISC1ELFParser> CreateRISC1ELFParser(const unsigned char *binary, size_t binary_size);

class Risc1ELF {
 public:
    static std::shared_ptr<Risc1ELF> CreateELF(std::shared_ptr<RISC1ELFParser> elf_parser);

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
    ~Risc1ELF() {}

 private:
    std::vector<uint8_t> m_binary;
    uint32_t m_entry_point;
    KernelMap m_kernels;
    Risc1ELF(std::shared_ptr<RISC1ELFParser> elf_parser);
};

struct Risc1SharedSection {
    int mapper_fd;
    size_t size;
    uint64_t vaddr;
};

class Risc1JobDesc;

class Risc1Job {
 public:
    static std::shared_ptr<Risc1Job> CreateRisc1Job(
                    std::shared_ptr<RISC1ELFParser> elf_parser,
                    std::shared_ptr<Risc1ELF> elf, int risc1_fd,
                    size_t stack_size);
    static std::shared_ptr<Risc1Job> CreateRisc1JobWithSharedSections(
                    std::shared_ptr<RISC1ELFParser> elf_parser,
                    std::shared_ptr<Risc1ELF> elf, int risc1_fd,
                    size_t stack_size, std::vector<Risc1SharedSection> sections);
    ~Risc1Job() {}

    const size_t GetStackSize() const;
    const int GetFD() const;

 private:
    Risc1Job(std::shared_ptr<RISC1ELFParser> elf_parser, std::shared_ptr<Risc1ELF> elf,
              int risc1_fd, size_t stack_size);
    Risc1Job(std::shared_ptr<RISC1ELFParser> elf_parser, std::shared_ptr<Risc1ELF> elf,
              int risc1_fd, size_t stack_size, std::vector<Risc1SharedSection> sections);
    std::shared_ptr<Risc1JobDesc> m_risc1_job_desc;
};

int Risc1JobInstancePoll(int job_instance_fd, int risc1_fd);

#endif  // _LIBRISC1_H_
