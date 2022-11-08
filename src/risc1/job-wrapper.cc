// Copyright 2021-2022 RnD Center "ELVEES", JSC

#include <cstring>
#include <ios>
#include <istream>
#include <list>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <iostream>

#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <elfio/elfio.hpp>

#include "librisc1.h"
#include "syscall.h"

#ifdef DRIVER_NOT_INSTALLED
#include <risc1.h>
#else
#include <linux/risc1.h>
#endif

static inline uint32_t risc_get_paddr(uint32_t addr)
{
	if (addr >= 0xc0000000)
		return addr;
	if (addr >= 0xa0000000)
		return addr - 0xa0000000;
	return addr & 0x7fffffff;
}

class ProgramSegment {
 public:
    ProgramSegment(uint32_t virtual_address, uint32_t size, uint32_t alignment, bool is_internal)
        : m_virtual_address(virtual_address), m_size(size), m_alignment(alignment),
          m_is_internal(is_internal) {}
    ~ProgramSegment() { }

    struct LoadSection {
        LoadSection(off_t offset, const void *data, uint32_t size)
            : m_offset(risc_get_paddr(offset)), m_data(data), m_size(size) {}
        off_t m_offset;
        const void *m_data;
        uint32_t m_size;
    };

    void AddLoadSection(off_t offset, const void *data, uint32_t size) {
        m_load_sections.push_back(LoadSection(offset, data, size));
    }

    void FillWithLoadSections(uint8_t *buffer) const {
        for (int i = 0; i < m_load_sections.size(); i++) {
            memcpy(buffer + m_load_sections[i].m_offset, m_load_sections[i].m_data,
                   m_load_sections[i].m_size);
        }
    }

    uint32_t m_virtual_address;
    uint32_t m_size;
    uint32_t m_alignment;
    bool m_is_internal;

 private:
    std::vector<LoadSection> m_load_sections;
};


static const uint32_t kPramStart = 0x03b00000;
static const uint32_t kPramEnd = kPramStart + 0x8000;

namespace {
class uchar_array_buffer : public std::streambuf {
 public:
    uchar_array_buffer(const unsigned char* data, unsigned int len)
        : begin_(data), end_(data + len), current_(data) {
        setg((char*)begin_, (char*)current_, (char*)end_);
    }

 private:
    virtual pos_type seekoff(off_type off, std::ios_base::seekdir way,
                             std::ios_base::openmode which = std::ios_base::in |
                                                             std::ios_base::out) {
        switch (way) {
            case std::ios_base::beg:
                current_ = begin_ + off;
                setg((char*)begin_, (char*)current_, (char*)end_);
                break;
            case std::ios_base::end:
                current_ = end_ + off;
                setg((char*)begin_, (char*)current_, (char*)end_);
                break;
            case std::ios_base::cur:
                current_ = current_ + off;
                setg((char*)begin_, (char*)current_, (char*)end_);
                break;
        }

        return pos_type(off_type(current_ - begin_));
    }

    virtual pos_type seekpos(pos_type sp, std::ios_base::openmode which = std::ios_base::in |
                                                                          std::ios_base::out) {
        current_ = begin_ + sp;
        setg((char*)begin_, (char*)current_, (char*)end_);
        return sp;
    }

    // Get area:
    std::streamsize showmanyc() { return end_ - current_; }

    int_type underflow() {
        if (current_ == end_) {
            return traits_type::eof();
        }
        return traits_type::to_int_type(*current_);
    }
    int_type uflow() {
        if (current_ == end_) {
            return traits_type::eof();
        }
        int_type res = traits_type::to_int_type(*current_++);
        setg((char*)begin_, (char*)current_, (char*)end_);
        return res;
    }

    // Putback:
    int_type pbackfail(int_type ch) {
        if (current_ == begin_ || (ch != traits_type::eof() && ch != current_[-1])) {
            return traits_type::eof();
        }
        int_type res = traits_type::to_int_type(*--current_);
        setg((char*)begin_, (char*)current_, (char*)end_);
        return res;
    }

    const unsigned char* const begin_;
    const unsigned char* const end_;
    const unsigned char* current_;
};

size_t gethugepagesize(void) {
    static size_t page_size = 0;

    if (page_size) return page_size;

    char *env = std::getenv("RISC1_HUGEPAGE_SECTIONS");
    if (env) {
        if (!strcmp(env, "0"))
            return page_size = getpagesize();
        if (strcmp(env, "1"))
            throw std::runtime_error("possible RISC1_HUGEPAGE_SECTIONS values: 0, 1");
    }

    char *line = NULL;
    size_t linelen = 0;
    FILE *f = fopen("/proc/meminfo", "r");

    if (!f)
          return 0;
    while (getline(&line, &linelen, f) > 0) {
       if (sscanf(line, "Hugepagesize:       %lu kB", &page_size) == 1) {
           page_size <<= 10;
           break;
       }
    }

    free(line);
    fclose(f);
    return page_size;
}
}

using namespace ELFIO;

class RISC1ELFParser {
 public:
    RISC1ELFParser(const unsigned char *elf, const size_t length);
    ~RISC1ELFParser() { };

    int AddSegmentToList(const int page_size, Elf64_Addr start_block_addr,
                         Elf64_Addr end_block_addr, Elf_Half seg_block_start,
                         Elf_Half seg_block_end);

    const std::list<ProgramSegment> &GetSegments() const { return m_segments; }
    elfio m_reader;  // reference to ELFIO::elfio
    std::list<ProgramSegment> m_segments;
    std::vector<uint8_t> m_binary;
    uint32_t m_entry_point;
    KernelMap m_kernels;
};

class Risc1JobDesc {
 public:
    Risc1JobDesc(std::shared_ptr<RISC1ELFParser> elf_parser, std::shared_ptr<Risc1ELF> elf,
                  int risc1_fd, size_t stack_size,
                  std::vector<Risc1SharedSection> shared_sections) {
        m_elf = elf;
        m_risc1_fd = risc1_fd;
        m_stack_size = stack_size;

        int hugepages = 1;
        char *env = std::getenv("RISC1_HUGEPAGE_ENABLE");
        if (env) {
            if (!strcmp(env, "0"))
                hugepages = 0;
            else if (strcmp(env, "1"))
                throw std::runtime_error("possible RISC1_HUGEPAGE_ENABLE values: 0, 1");
        }
        int i = 0; int ret;
        struct risc1_buf buf;
        for (auto it = elf_parser->GetSegments().cbegin(); it != elf_parser->GetSegments().cend();
             ++it, ++i) {
            m_sections.resize(i + 1);
            if (posix_memalign(reinterpret_cast<void**>(&m_sections[i]), it->m_alignment,
                               it->m_size) != 0)
                throw std::runtime_error("posix_memalign failed");
            if (!it->m_is_internal) madvise(m_sections[i], it->m_size, MADV_HUGEPAGE);
            // Copying PT_LOAD data to section
            it->FillWithLoadSections(m_sections[i]);
            buf.type = RISC1_CACHED_BUFFER_FROM_UPTR;
            buf.size = it->m_size;
            buf.p = reinterpret_cast<uintptr_t>(m_sections[i]);
            ret = ioctl(m_risc1_fd, RISC1_IOC_CREATE_BUFFER, &buf);
            if (ret != 0) throw std::runtime_error("RISC1_IOC_CREATE_BUFFER failed");
            ret = ioctl(m_risc1_fd, RISC1_IOC_CREATE_MAPPER, &buf);
            if (ret != 0) throw std::runtime_error("RISC1_IOC_CREATE_MAPPER failed");
            m_job.elf_sections[i].type = RISC1_ELF_SECTION_DATA;
            m_job.elf_sections[i].size = it->m_size;
            m_job.elf_sections[i].risc1_virtual_address = it->m_virtual_address;
            m_job.elf_sections[i].mapper_fd = buf.mapper_fd;
            if (close(buf.dmabuf_fd)) throw std::runtime_error("close dmabuf failed");
        }
        m_job.num_elf_sections = elf_parser->GetSegments().size();

        for (int i = 0; i < shared_sections.size(); ++i) {
            m_job.elf_sections[m_job.num_elf_sections].type = RISC1_ELF_SECTION_DATA;
            m_job.elf_sections[m_job.num_elf_sections].size = shared_sections[i].size;
            m_job.elf_sections[m_job.num_elf_sections].risc1_virtual_address =
                                                              shared_sections[i].vaddr;
            m_job.elf_sections[m_job.num_elf_sections].mapper_fd = shared_sections[i].mapper_fd;
            m_job.num_elf_sections++;
        }
        m_shared_sections_num = shared_sections.size();

        int page_size = gethugepagesize();

        buf.type = RISC1_CACHED_BUFFER_FROM_UPTR;
        buf.size = m_stack_size;
        if (posix_memalign(reinterpret_cast<void**>(&m_job_stack), page_size, buf.size) != 0)
            throw std::runtime_error("posix_memalign failed");
        madvise(m_job_stack, buf.size, MADV_HUGEPAGE);
        buf.p = reinterpret_cast<uintptr_t>(m_job_stack);
        ret = ioctl(m_risc1_fd, RISC1_IOC_CREATE_BUFFER, &buf);
        if (ret != 0) throw std::runtime_error("RISC1_IOC_CREATE_BUFFER failed");
        ret = ioctl(m_risc1_fd, RISC1_IOC_CREATE_MAPPER, &buf);
        if (ret != 0) throw std::runtime_error("RISC1_IOC_CREATE_MAPPER failed");
        m_job.stack_fd = buf.mapper_fd;
        if (close(buf.dmabuf_fd)) throw std::runtime_error("close dmabuf failed");
        m_job.hugepages = hugepages;
        ret = ioctl(m_risc1_fd, RISC1_IOC_CREATE_JOB, &m_job);
        if (ret != 0) throw std::runtime_error("RISC1_IOC_CREATE_JOB failed");
    }

    ~Risc1JobDesc() {
        for (int i = 0; i < m_job.num_elf_sections - m_shared_sections_num; ++i) {
            if (close(m_job.elf_sections[i].mapper_fd))
                 throw std::runtime_error("Failed to close section mapper_fd");
            free(m_sections[i]);
        }

        free(m_job_stack);
        if (close(m_job.stack_fd)) throw std::runtime_error("Failed to close stack_fd");
        if (close(m_job.job_fd)) throw std::runtime_error("Failed to close job");
    }

    std::vector<uint8_t*> m_sections;
    size_t m_stack_size;
    std::shared_ptr<Risc1ELF> m_elf;
    void * m_job_stack;
    size_t m_shared_sections_num;
    risc1_job m_job;
    int m_risc1_fd;
};

int RISC1ELFParser::AddSegmentToList(const int page_size, Elf64_Addr start_block_addr,
                                 Elf64_Addr end_block_addr, Elf_Half seg_block_start,
                                 Elf_Half seg_block_end) {
    uint32_t alignment = page_size;
    bool is_internal_mem = false;
    if (start_block_addr >= kPramStart && end_block_addr <= kPramEnd) {
        // this segment intersects PRAM
        if (start_block_addr != kPramStart) return -1;
        alignment = getpagesize();
        is_internal_mem = true;
    }
    const size_t block_size = end_block_addr - start_block_addr;

    ProgramSegment segment =
        ProgramSegment(start_block_addr, block_size, alignment, is_internal_mem);
    for (Elf_Half seg_to_copy = seg_block_start; seg_to_copy < seg_block_end; ++seg_to_copy) {
        if (m_reader.segments[seg_to_copy]->get_type() == PT_LOAD) {
            segment.AddLoadSection(
                m_reader.segments[seg_to_copy]->get_virtual_address() - start_block_addr,
                m_reader.segments[seg_to_copy]->get_data(),
                m_reader.segments[seg_to_copy]->get_file_size());
        }
    }
    m_segments.emplace_back(segment);
    return 0;
}

RISC1ELFParser::RISC1ELFParser(const unsigned char *elf, const size_t length) {
    int result;
    int page_size = getpagesize();
    int hugepage_size = gethugepagesize();
    m_binary.assign(elf, elf + length);

    uchar_array_buffer input_source(elf, length);
    std::istream input_stream(&input_source);

    if (!m_reader.load(input_stream))
        throw std::runtime_error("could not load elf file");

    // @TODO: check m_reader.get_machine() and other fields of ELF header

    // fill segments
    Elf64_Addr end_block_addr = page_size;
    Elf64_Addr start_block_addr = 0;
    Elf_Half seg_block_start = 0;
    bool first_block = true;
    Elf_Half seg;

    for (seg = 0; seg < m_reader.segments.size(); ++seg) {
        // find continous block of segments respect to hugepage_size alignment
        if (m_reader.segments[seg]->get_type() == PT_LOAD) {
            Elf64_Addr virt_addr = risc_get_paddr(m_reader.segments[seg]->get_virtual_address());
            Elf_Xword mem_size = m_reader.segments[seg]->get_memory_size();
            Elf64_Addr aligned_addr;
            Elf64_Addr aligned_end_addr;
            // @TODO: correctly process blocks near protected regions.
            if (virt_addr < kPramEnd &&
                (virt_addr + mem_size) >= kPramStart) {
                if (virt_addr != kPramStart)
                    throw std::runtime_error("Invalid binary");

                aligned_addr = (virt_addr / page_size) * page_size;
                aligned_end_addr = ((virt_addr + mem_size) / page_size + 1) * page_size;
                if (aligned_end_addr > kPramEnd)
                    aligned_end_addr = kPramEnd;
            } else {
                aligned_addr = (virt_addr / hugepage_size) * hugepage_size;
                aligned_end_addr = ((virt_addr + mem_size) / hugepage_size + 1) *
                                   hugepage_size;
                if (aligned_addr < kPramEnd &&
                    aligned_end_addr >= kPramStart)
                {
                    aligned_addr = (virt_addr / page_size) * page_size;
                    aligned_end_addr = ((virt_addr + mem_size) / page_size + 1) * page_size;
                }
            }
            if (first_block || aligned_addr > end_block_addr) {
                if (!first_block) {
                    result = AddSegmentToList(hugepage_size, start_block_addr,
                                              end_block_addr, seg_block_start, seg);
                    if (result)
                        throw std::runtime_error("Fail to add segment");
                }
                seg_block_start = seg;
                start_block_addr = aligned_addr;
                first_block = false;
            }
            end_block_addr = aligned_end_addr;
        }
    }
    if (seg > 0) {
        result = AddSegmentToList(hugepage_size, start_block_addr,
                                  end_block_addr, seg_block_start, seg);
        if (result)
            throw std::runtime_error("Fail to add segment");
    } else
            throw std::runtime_error("Invalid binary");

    for (Elf_Half sec = 0; sec < m_reader.sections.size(); ++sec) {
        section* psec = m_reader.sections[sec];
        if (psec->get_type() == SHT_DYNSYM || psec->get_type() == SHT_RELA ||
            psec->get_type() == SHT_DYNAMIC || psec->get_type() == SHT_REL) {
            throw std::runtime_error("Invalid binary");
        }
        std::string str = psec->get_name();
        if (psec->get_type() == SHT_SYMTAB) {
            const symbol_section_accessor symbols(m_reader, psec);
            for (unsigned int j = 0; j < symbols.get_symbols_num(); ++j) {
                std::string name;
                Elf64_Addr value;
                Elf_Xword size;
                unsigned char bind;
                unsigned char type;
                Elf_Half section_index;
                unsigned char other;
                symbols.get_symbol(j, name, value, size, bind, type, section_index, other);
                if ((type & STT_FUNC) && (bind & STB_GLOBAL) && other == STV_DEFAULT) {
                    m_kernels[name] = value;
                }
            }
        }
    }
    m_entry_point = m_reader.get_entry();
}

std::shared_ptr<RISC1ELFParser> CreateRISC1ELFParser(const unsigned char *binary, size_t binary_size) {
    return std::shared_ptr<RISC1ELFParser>(new RISC1ELFParser(binary, binary_size));
}

Risc1ELF::Risc1ELF(std::shared_ptr<RISC1ELFParser> elf_parser) {
    m_binary = elf_parser->m_binary;
    m_entry_point = elf_parser->m_entry_point;
    m_kernels = elf_parser->m_kernels;
}

std::shared_ptr<Risc1ELF> Risc1ELF::CreateELF(std::shared_ptr<RISC1ELFParser> elf_parser) {
    return std::shared_ptr<Risc1ELF>(new Risc1ELF(elf_parser));
}

Risc1Job::Risc1Job(std::shared_ptr<RISC1ELFParser> elf_parser, std::shared_ptr<Risc1ELF> elf,
                     int risc1_fd, size_t stack_size) {
    std::vector<Risc1SharedSection> empty_vector;
    m_risc1_job_desc = std::shared_ptr<Risc1JobDesc>(
        new Risc1JobDesc(elf_parser, elf, risc1_fd, stack_size, empty_vector));
}

std::shared_ptr<Risc1Job> Risc1Job::CreateRisc1Job(std::shared_ptr<RISC1ELFParser> elf_parser,
                                                      std::shared_ptr<Risc1ELF> elf,
                                                      int risc1_fd,
                                                      size_t stack_size) {
    return std::shared_ptr<Risc1Job>(
        new Risc1Job(elf_parser, elf, risc1_fd, stack_size));
}

Risc1Job::Risc1Job(std::shared_ptr<RISC1ELFParser> elf_parser, std::shared_ptr<Risc1ELF> elf,
                     int risc1_fd, size_t stack_size, std::vector<Risc1SharedSection> sections) {
    m_risc1_job_desc = std::shared_ptr<Risc1JobDesc>(
        new Risc1JobDesc(elf_parser, elf, risc1_fd, stack_size, sections));
}

std::shared_ptr<Risc1Job> Risc1Job::CreateRisc1JobWithSharedSections(
                    std::shared_ptr<RISC1ELFParser> elf_parser,
                    std::shared_ptr<Risc1ELF> elf, int risc1_fd,
                    size_t stack_size, std::vector<Risc1SharedSection> sections) {
    return std::shared_ptr<Risc1Job>(
        new Risc1Job(elf_parser, elf, risc1_fd, stack_size, sections));
}

const size_t Risc1Job::GetStackSize() const {
    return m_risc1_job_desc->m_stack_size;
}
const int Risc1Job::GetFD() const {
    return m_risc1_job_desc->m_job.job_fd;
}

int Risc1JobInstancePoll(int job_instance_fd, int risc1_fd) {
    risc1_job_instance_status job_status;
    struct pollfd fds;

    job_status.job_instance_fd = job_instance_fd;
    fds.fd = job_instance_fd;
    fds.events = POLLIN;

    while (1) {
        if (poll(&fds, 1, -1) <= 0) {
            throw std::runtime_error("poll errored out or timed out");
        }
        if (fds.revents & (POLLERR | POLLHUP | POLLNVAL)) {
            throw std::runtime_error("poll returned error");
        }

        ioctl(risc1_fd, RISC1_IOC_GET_JOB_STATUS, &job_status);
        if (job_status.state != RISC1_JOB_STATUS_SYSCALL)
            break;
        Risc1SyscallHandler(job_instance_fd, risc1_fd);
    }

    Risc1FlushPreamble();
    return job_status.error;
}
