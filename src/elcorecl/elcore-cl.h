// Copyright 2018-2022 RnD Center "ELVEES", JSC

#pragma once
#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstring>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <stdexcept>
#include <thread>
#include <vector>
#include <utility>

#include <boost/intrusive_ptr.hpp>

#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <elcorecl/elcorecl.h>
#ifdef DRIVER_NOT_INSTALLED
#include <elcore50.h>
#else
#include <linux/elcore50.h>
#include <linux/risc1.h>
#endif

#include "libelcore.h"
#include "librisc1.h"

#include <linux/dma-buf.h>

#define E50_DEFAULT_STACK_SIZE      (2048 * 1024)
#define RISC1_DEFAULT_STACK_SIZE    (32 * 4096)

struct EclPlatformId;

struct _ecl_platform_id {
    unsigned long id;
    EclPlatformId *eid;
};

struct _ecl_object {
    explicit _ecl_object() : m_ref_count(0) {}
    virtual ~_ecl_object() {}
    virtual int add_ref() { return ++m_ref_count; }
    virtual int release() {
        int count = --m_ref_count;
        if (count == 0) delete this;
        return count;
    }
    int ref_count() { return m_ref_count; }

 private:
    std::atomic_int m_ref_count;
};

void intrusive_ptr_add_ref(_ecl_object *p);

void intrusive_ptr_release(_ecl_object *p);

class elcore50 {
public:
    static const int EclPlId = 0;
    static std::string filter() { return "elcore\\d+"; }
    static std::string job_launcher() { return "_elcore_job_launcher"; }
    typedef struct elcore50_job_instance job_instance;
    typedef struct elcore50_device_info device_info;
    typedef struct elcore50_job job;
    typedef struct elcore50_job_instance_status job_instance_status;
    typedef struct elcore50_buf buf;
    typedef struct elcore50_buf_sync buf_sync;

    typedef class ELFParser parser;
    typedef class ElcoreELF ELF;
    typedef class ElcoreJob Job;
    typedef class ElcoreSharedSection SharedSection;

    static std::shared_ptr<ELFParser> CreateELFParser(
        const unsigned char *binary, size_t binary_size)
    {
        return ::CreateELFParser(binary, binary_size);
    }

    static std::shared_ptr<ElcoreJob> CreateJob(std::shared_ptr<ELFParser> elf_parser,
                                                      std::shared_ptr<ElcoreELF> elf,
                                                      int elcore_fd,
                                                      size_t stack_size)
    {
        return ElcoreJob::CreateElcoreJob(elf_parser, elf, elcore_fd, stack_size);
    }

    static std::shared_ptr<ElcoreJob> CreateJobWithSharedSections(
                    std::shared_ptr<ELFParser> elf_parser,
                    std::shared_ptr<ElcoreELF> elf, int elcore_fd,
                    size_t stack_size, std::vector<ElcoreSharedSection> sections)
    {
        return ElcoreJob::CreateElcoreJobWithSharedSections(
                    elf_parser, elf, elcore_fd, stack_size, sections);
    }

    static int JobInstancePoll(int job_instance_fd, int elcore_fd) {
        return ElcoreJobInstancePoll(job_instance_fd, elcore_fd);
    }

    static void fix_job_instance(job_instance &instance) { }

    static const int IOC_GET_CORE_IDX = ELCORE50_IOC_GET_CORE_IDX;
    static const int IOC_CREATE_JOB = ELCORE50_IOC_CREATE_JOB;
    static const int IOC_ENQUEUE_JOB = ELCORE50_IOC_ENQUEUE_JOB;
    static const int IOC_GET_JOB_STATUS = ELCORE50_IOC_GET_JOB_STATUS;
    static const int IOC_CREATE_MAPPER = ELCORE50_IOC_CREATE_MAPPER;
    static const int IOC_SYNC_BUFFER = ELCORE50_IOC_SYNC_BUFFER;
    static const int IOC_CREATE_BUFFER = ELCORE50_IOC_CREATE_BUFFER;

    static const size_t DEFAULT_STACK_SIZE = E50_DEFAULT_STACK_SIZE;

    static const enum elcore50_job_arg_type TYPE_GLOBAL_MEMORY = ELCORE50_TYPE_GLOBAL_MEMORY;
    static const enum elcore50_job_arg_type TYPE_NC_GLOBAL_MEMORY = ELCORE50_TYPE_NC_GLOBAL_MEMORY;
    static const enum elcore50_job_arg_type TYPE_LOCAL_MEMORY = ELCORE50_TYPE_LOCAL_MEMORY;
    static const enum elcore50_job_arg_type TYPE_BASIC = ELCORE50_TYPE_BASIC;
    static const enum elcore50_job_arg_type TYPE_DMA_MEMORY = ELCORE50_TYPE_DMA_MEMORY;

    static const enum elcore50_buf_sync_dir BUF_SYNC_DIR_TO_CPU = ELCORE50_BUF_SYNC_DIR_TO_CPU;
    static const enum elcore50_buf_sync_dir BUF_SYNC_DIR_TO_DEVICE = ELCORE50_BUF_SYNC_DIR_TO_DEVICE;

    static const enum elcore50_buf_type NONCACHED_BUFFER = ELCORE50_NONCACHED_BUFFER;
    static const enum elcore50_buf_type CACHED_BUFFER_FROM_UPTR = ELCORE50_CACHED_BUFFER_FROM_UPTR;

    static const int JOB_STATUS_DONE = ELCORE50_JOB_STATUS_DONE;
    static const int JOB_STATUS_SUCCESS = ELCORE50_JOB_STATUS_SUCCESS;
};

class risc1 {
public:
    static const int EclPlId = 1;
    static std::string filter() { return "risc1"; }
    static std::string job_launcher() { return "_reset"; }
    typedef struct risc1_job_instance job_instance;
    typedef struct risc1_device_info device_info;
    typedef struct risc1_job job;
    typedef struct risc1_job_instance_status job_instance_status;
    typedef struct risc1_buf buf;
    typedef struct risc1_buf_sync buf_sync;

    typedef class RISC1ELFParser parser;
    typedef class Risc1ELF ELF;
    typedef class Risc1Job Job;
    typedef class Risc1SharedSection SharedSection;

    static std::shared_ptr<RISC1ELFParser> CreateELFParser(
        const unsigned char *binary, size_t binary_size)
    {
        return ::CreateRISC1ELFParser(binary, binary_size);
    }

    static std::shared_ptr<Risc1Job> CreateJob(std::shared_ptr<RISC1ELFParser> elf_parser,
                                                      std::shared_ptr<Risc1ELF> elf,
                                                      int elcore_fd,
                                                      size_t stack_size)
    {
        return Risc1Job::CreateRisc1Job(elf_parser, elf, elcore_fd, stack_size);
    }

    static std::shared_ptr<Risc1Job> CreateJobWithSharedSections(
                    std::shared_ptr<RISC1ELFParser> elf_parser,
                    std::shared_ptr<Risc1ELF> elf, int elcore_fd,
                    size_t stack_size, std::vector<Risc1SharedSection> sections)
    {
        return Risc1Job::CreateRisc1JobWithSharedSections(
                    elf_parser, elf, elcore_fd, stack_size, sections);
    }

    static int JobInstancePoll(int job_instance_fd, int risc1_fd) {
        return Risc1JobInstancePoll(job_instance_fd, risc1_fd);
    }

    static void fix_job_instance(job_instance &instance) {
        instance.catch_mode = 0x00bb0003;
    }

    static const int IOC_GET_CORE_IDX = RISC1_IOC_GET_CORE_IDX;
    static const int IOC_CREATE_JOB = RISC1_IOC_CREATE_JOB;
    static const int IOC_ENQUEUE_JOB = RISC1_IOC_ENQUEUE_JOB;
    static const int IOC_GET_JOB_STATUS = RISC1_IOC_GET_JOB_STATUS;
    static const int IOC_CREATE_MAPPER = RISC1_IOC_CREATE_MAPPER;
    static const int IOC_SYNC_BUFFER = RISC1_IOC_SYNC_BUFFER;
    static const int IOC_CREATE_BUFFER = RISC1_IOC_CREATE_BUFFER;

    static const size_t DEFAULT_STACK_SIZE = RISC1_DEFAULT_STACK_SIZE;

    static const enum risc1_job_arg_type TYPE_GLOBAL_MEMORY = RISC1_TYPE_GLOBAL_MEMORY;
    static const enum risc1_job_arg_type TYPE_NC_GLOBAL_MEMORY = RISC1_TYPE_NC_GLOBAL_MEMORY;
    static const enum risc1_job_arg_type TYPE_LOCAL_MEMORY = RISC1_TYPE_LOCAL_MEMORY;
    static const enum risc1_job_arg_type TYPE_BASIC = RISC1_TYPE_BASIC;
    static const enum risc1_job_arg_type TYPE_DMA_MEMORY = RISC1_TYPE_DMA_MEMORY;

    static const enum risc1_buf_sync_dir BUF_SYNC_DIR_TO_CPU = RISC1_BUF_SYNC_DIR_TO_CPU;
    static const enum risc1_buf_sync_dir BUF_SYNC_DIR_TO_DEVICE = RISC1_BUF_SYNC_DIR_TO_DEVICE;

    static const enum risc1_buf_type NONCACHED_BUFFER = RISC1_NONCACHED_BUFFER;
    static const enum risc1_buf_type CACHED_BUFFER_FROM_UPTR = RISC1_CACHED_BUFFER_FROM_UPTR;

    static const int JOB_STATUS_DONE = RISC1_JOB_STATUS_DONE;
    static const int JOB_STATUS_SUCCESS = RISC1_JOB_STATUS_SUCCESS;
};

#ifdef EMULATE_DEVICES
template <typename T>
struct _ecl_device {
    _ecl_device(const std::string &) {}
    ~_ecl_device() {}
    virtual int getPlatform() const {
        return T::EclPlId;
    }
    int enqueue_job(T::job_instance *) { return 0; }
    int get_job_status(T::job_instance_status *job_status) {
        job_status->state = T::JOB_STATUS_DONE;
        job_status->error = T::JOB_STATUS_SUCCESS;
        return 0;
    }
    void poll_job(int job_fd) {}
    int close_job(int job_fd) { return 0; }
};
#else
template <typename T>
struct _ecl_device {
    _ecl_device(const std::string &dev_name) {
        m_fd = open(dev_name.c_str(), O_RDONLY, 0);
        if (m_fd == -1) throw std::runtime_error("can't open device");
        struct T::device_info info;
        ioctl(m_fd, T::IOC_GET_CORE_IDX, &info);
        m_devnum = info.cluster_id * info.cluster_cap + info.core_in_cluster_id;
    }
    ~_ecl_device() { close(); }
    virtual int getPlatform() const {
        return T::EclPlId;
    }
    int create_job(typename T::job *job) {
        return ioctl(m_fd, T::IOC_CREATE_JOB, job);
    }
    int enqueue_job(typename T::job_instance *job) {
        return ioctl(m_fd, T::IOC_ENQUEUE_JOB, job);
    }
    int get_job_status(typename T::job_instance_status *job_status) {
        int ret = ioctl(m_fd, T::IOC_GET_JOB_STATUS, job_status);
        if (ret) throw std::runtime_error("IOC_GET_JOB_STATUS failed");
        return ret;
    }
    int create_buffer(typename T::buf *buf) {
        int ret = ioctl(m_fd, T::IOC_CREATE_BUFFER, buf);
        if (ret) throw std::runtime_error("IOC_CREATE_BUFFER failed");
        return ret;
    }
    int create_mapper(typename T::buf *buf) {
        int ret = ioctl(m_fd, T::IOC_CREATE_MAPPER, buf);
        if (ret) throw std::runtime_error("IOC_CREATE_MAPPER failed");
        return ret;
    }
    int sync_for_host(typename T::buf_sync *sync) {
        sync->dir = T::BUF_SYNC_DIR_TO_CPU;
        int ret = ioctl(m_fd, T::IOC_SYNC_BUFFER, sync);
        if (ret) throw std::runtime_error("IOC_SYNC_BUFFER failed");
        return ret;
    }
    int sync_for_device(struct T::buf_sync *sync) {
        sync->dir = T::BUF_SYNC_DIR_TO_DEVICE;
        int ret = ioctl(m_fd, T::IOC_SYNC_BUFFER, sync);
        if (ret) throw std::runtime_error("IOC_SYNC_BUFFER failed");
        return ret;
    }
    void poll_job(int job_fd) {
        struct pollfd fds;
        fds.fd = job_fd;
        fds.events = POLLIN;
        if (poll(&fds, 1, -1) <= 0) {
            throw std::runtime_error("poll errored out or timed out");
        }
        if (fds.revents & (POLLERR | POLLHUP | POLLNVAL)) {
            throw std::runtime_error("poll returned error");
        }
    }
    int close_job(int job_fd) {
        int ret;
        if (ret = ::close(job_fd)) throw std::runtime_error("close job failed");
        return ret;
    }

    int m_devnum;
    int m_fd;

 private:
    int close() {
        int ret;
        if (ret = ::close(m_fd)) throw std::runtime_error("close device failed");
        return ret;
    }
};
#endif

template <typename T>
struct _ecl_device_idT;

struct _ecl_device_id {
    _ecl_device_id(const std::string &dev_name) : m_dev_name(dev_name)/*, m_dev(nullptr)*/ {}

    template <typename T>
    std::shared_ptr<_ecl_device<T>> open_device() {
        _ecl_device_idT<T> *idt = reinterpret_cast<_ecl_device_idT<T>*>(this);
        return idt->open_device();
    }

    virtual int getPlatform() const = 0;

 protected:
    const std::string m_dev_name;

    std::mutex m_mutex;
};

template <typename T>
struct _ecl_device_idT : public _ecl_device_id {

    _ecl_device_idT(const std::string &dev_name) : _ecl_device_id(dev_name),  m_dev(nullptr) {}
    std::shared_ptr<_ecl_device<T>> open_device() {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_dev == nullptr)
            m_dev = std::shared_ptr<_ecl_device<T>>(new _ecl_device<T>(m_dev_name));
        return m_dev;
    }

    virtual int getPlatform() const {
        return T::EclPlId;
    }

    std::shared_ptr<_ecl_device<T>> m_dev;
};

template <typename T>
bool IsDeviceValid(ecl_device_id device_id);

template <typename T>
struct _ecl_contextT;

struct _ecl_context : public _ecl_object {
    _ecl_context(void(ECL_CALLBACK *pfn_notify)(const char *, const void *, size_t, void *),
                 void *user_data)
        : m_pfn_notify(pfn_notify), m_user_data(user_data) {
    }

    virtual int getPlatform() const = 0;

    template <typename T>
    std::vector<std::shared_ptr<_ecl_device<T>>> &getDevices() {
        _ecl_contextT<T> *idt = reinterpret_cast<_ecl_contextT<T>*>(this);
        return idt->m_devices;
    }

    std::vector<_ecl_device_id *> m_device_ids;

 protected:
    void(ECL_CALLBACK *m_pfn_notify)(const char *, const void *, size_t, void *);
    void *m_user_data;
};

template <typename T>
struct _ecl_contextT : public _ecl_context {
    _ecl_contextT(std::vector<_ecl_device_id *> &&device_ids,
                 void(ECL_CALLBACK *pfn_notify)(const char *, const void *, size_t, void *),
                 void *user_data)
            : _ecl_context(pfn_notify, user_data) {
            m_device_ids = device_ids;
            for (_ecl_device_id *device_id : m_device_ids)
                m_devices.push_back(device_id->open_device<T>());
    }

    virtual int getPlatform() const {
        return T::EclPlId;
    }

    std::vector<std::shared_ptr<_ecl_device<T>>> m_devices;
};

template <typename T>
struct _ecl_kernel_instance : _ecl_object {
    typename T::job_instance m_job_instance;
    typename T::job_instance_status m_job_status;
};

struct _ecl_sync_object_instance : _ecl_object {
    ecl_mem m_memobj;
    ecl_map_flags m_map_flags;
    size_t m_offset;
    size_t m_cb;
};

template <typename T>
struct _ecl_command_queueT;

template <typename T>
struct ecl_command_queue_node : _ecl_object {
    boost::intrusive_ptr<_ecl_event> m_event;
    std::vector<boost::intrusive_ptr<_ecl_event>> m_wait_list;
    boost::intrusive_ptr<_ecl_command_queueT<T>> m_queue;
    boost::intrusive_ptr<_ecl_kernel_instance<T>> m_kernel;
    std::list<std::vector<uint8_t>> m_basic_args_copy;
    std::list<boost::intrusive_ptr<_ecl_mem>> m_mem_args_ref;
    boost::intrusive_ptr<_ecl_sync_object_instance> m_sync_object;
};

struct _ecl_command_queue : public _ecl_object {
    _ecl_command_queue(_ecl_device_id *device_id, _ecl_context *context,
                       ecl_command_queue_properties properties)
        : m_device_id(device_id), m_context(context), m_properties(properties) {
        // Поток PollThread выходит при значении reference counter'а <= 1.
        // Конструктор по умолчанию для intrusive_ptr вызывает add_ref только после конструктора
        // _ecl_command_queue. То есть на старте потока может оказаться, что счётчик ссылок равен
        // единице. Поэтому до старта потока сразу выставляется значение счётчка ссылок равное 2
        // и требуется, чтобы использовался не конструктор intrusive_ptr по умолчанию, а
        // его версия, которая не инкрементирует счётчик.
        // Таким образом: на старте счётчик равен 2. Когда пользователь сделает eclRelease,
        // счётчик станет равен 1, после этого поток PollThread сделает release() и объект
        // уничтожится.
        add_ref();
        add_ref();
    }
    ~_ecl_command_queue() {}

    virtual int getPlatform() const = 0;

    _ecl_device_id *const m_device_id;
    const boost::intrusive_ptr<_ecl_context> m_context;
    const ecl_command_queue_properties m_properties;

    virtual void PollThread() = 0;

    virtual int release() {
        int count = _ecl_object::release();
        if (count != 0) m_commands_list_cond.notify_all();
        return count;
    }

    std::mutex m_commands_list_guard;
    std::condition_variable m_commands_list_cond;
};

template <typename T>
struct _ecl_command_queueT : public _ecl_command_queue {
    _ecl_command_queueT(_ecl_device_id *device_id, _ecl_context *context,
                       ecl_command_queue_properties properties)
        : _ecl_command_queue(device_id, context, properties) {
            m_device = m_device_id->open_device<T>();
            std::thread t(&_ecl_command_queue::PollThread, this);
            t.detach();
        }

    virtual int getPlatform() const {
        return T::EclPlId;
    }

    virtual void PollThread();

    std::shared_ptr<_ecl_device<T>> m_device;
    std::list<boost::intrusive_ptr<ecl_command_queue_node<T>>> m_submitted_kernels_list;
    boost::intrusive_ptr<ecl_command_queue_node<T>> m_last_command_in_queue;
};


struct _ecl_mem : public _ecl_object {
    _ecl_mem(ecl_context context, ecl_mem_flags flags, size_t size, void *host_ptr,
             ecl_mem_object_type type, int dma_buf_fd = -1)
        : m_context(context), m_flags(flags), m_size(size), m_host_ptr(host_ptr), m_type(type) {
    }
    virtual ~_ecl_mem() {}
    virtual int sync_for_host(size_t offset, size_t cb, ecl_map_flags map_flags) = 0;
    virtual int sync_for_device() = 0;

    virtual int getPlatform() const = 0;

    const ecl_mem_object_type m_type;
    const ecl_mem_flags m_flags;
    const size_t m_size;
    void *m_host_ptr;
    std::list<std::pair<void(ECL_CALLBACK *)(ecl_mem, void *), void *>> m_destructor_callbacks;
    int mapper_fd;
    const boost::intrusive_ptr<_ecl_context> m_context;
};

template <typename T>
struct _ecl_memT : public _ecl_mem {
    _ecl_memT(ecl_context context, ecl_mem_flags flags, size_t size, void *host_ptr,
             ecl_mem_object_type type, int dma_buf_fd = -1)
        : _ecl_mem(context, flags, size, host_ptr, type, dma_buf_fd) {
        m_mem_info.size = m_size;
        m_dev = m_context->getDevices<T>()[0];
        if (flags & ECL_MEM_USE_DMABUF) {
            m_mem_info.type = T::NONCACHED_BUFFER;
            m_mem_info.dmabuf_fd = dma_buf_fd;
            m_host_ptr = mmap(NULL, m_size, PROT_READ | PROT_WRITE, MAP_SHARED,
                              m_mem_info.dmabuf_fd, 0);
            if (m_host_ptr == MAP_FAILED)
                m_host_ptr = nullptr;
        } else {
            m_mem_info.mapper_fd = m_mem_info.dmabuf_fd = 0;
            m_mem_info.type = T::CACHED_BUFFER_FROM_UPTR;
            m_mem_info.p = reinterpret_cast<__u64>(m_host_ptr);
            m_dev->create_buffer(&m_mem_info);
        }

        m_dev->create_mapper(&m_mem_info);
        mapper_fd = m_mem_info.mapper_fd;
        m_map_flags = 0;
    }
    virtual ~_ecl_memT() {
        if (m_host_ptr && (m_flags & ECL_MEM_USE_DMABUF)) {
            munmap(m_host_ptr, m_size);
            m_host_ptr = nullptr;
        }
        if (!(m_flags & ECL_MEM_USE_DMABUF))
            if (close(m_mem_info.dmabuf_fd)) throw std::runtime_error("close mem dmabuf_fd failed");
        if (close(m_mem_info.mapper_fd)) throw std::runtime_error("close mem mapper_fd failed");
        for (auto it = m_destructor_callbacks.crbegin(); it != m_destructor_callbacks.crend();
             ++it) {
            it->first(this, it->second);
        }
    }
    virtual int sync_for_host(size_t offset, size_t cb, ecl_map_flags map_flags) {
        int ret = 0;
        m_map_flags = map_flags;
        if (m_flags & ECL_MEM_USE_DMABUF) {
            struct dma_buf_sync sync = {0};
            sync.flags = DMA_BUF_SYNC_START;
            if (map_flags & ECL_MAP_READ) sync.flags |= DMA_BUF_SYNC_READ;
            if (map_flags & ECL_MAP_WRITE) sync.flags |= DMA_BUF_SYNC_WRITE;
            ret = ioctl(m_mem_info.dmabuf_fd, DMA_BUF_IOCTL_SYNC, &sync);
        } else {
            m_mem_sync.mapper_fd = m_mem_info.mapper_fd;
            m_mem_sync.offset = offset;
            m_mem_sync.size = cb;

            ret = m_dev->sync_for_host(&m_mem_sync);
        }
        return ret;
    }
    virtual int sync_for_device() {
        int ret = 0;
        if (m_flags & ECL_MEM_USE_DMABUF) {
            struct dma_buf_sync sync = {0};
            sync.flags = DMA_BUF_SYNC_END;
            if (m_map_flags & ECL_MAP_READ) sync.flags |= DMA_BUF_SYNC_READ;
            if (m_map_flags & ECL_MAP_WRITE) sync.flags |= DMA_BUF_SYNC_WRITE;
            ret = ioctl(m_mem_info.dmabuf_fd, DMA_BUF_IOCTL_SYNC, &sync);
        } else
            if (m_map_flags & ECL_MAP_WRITE) ret = m_dev->sync_for_device(&m_mem_sync);
        m_map_flags = 0;
        return ret;
    }

    virtual int getPlatform() const {
        return T::EclPlId;
    }

    ecl_map_flags m_map_flags;
    typename T::buf m_mem_info;
    typename T::buf_sync m_mem_sync;
    std::shared_ptr<_ecl_device<T>> m_dev;
};

template <typename T>
struct _ecl_programT;

struct _ecl_program : public _ecl_object {
    _ecl_program(ecl_context context, ecl_uint num_devices, const ecl_device_id *devices)
        : m_context(context),
          m_devices(devices, devices + num_devices) {
        m_build_status = ECL_BUILD_NONE;
    }
    virtual ~_ecl_program() {
        for (auto it = m_release_callbacks.crbegin(); it != m_release_callbacks.crend(); ++it) {
            it->first(this, it->second);
        }
    }

    virtual int getPlatform() const = 0;

    template <typename T>
    std::shared_ptr<typename T::ELF> &getELF() {
        _ecl_programT<T> *idt = reinterpret_cast<_ecl_programT<T>*>(this);
       return idt->m_elf;
    }

    const size_t get_stack_size() { return m_stack_size; }
    const boost::intrusive_ptr<_ecl_context> m_context;
    std::list<std::pair<void(ECL_CALLBACK *)(ecl_program, void *), void *>> m_release_callbacks;
    std::vector<_ecl_device_id *> m_devices;
    ecl_build_status m_build_status;
    std::vector<boost::intrusive_ptr<_ecl_mem>> m_shared_sections;
    size_t m_stack_size;
};

template <typename T>
struct _ecl_programT : public _ecl_program {
   _ecl_programT(ecl_context context, ecl_uint num_devices, const ecl_device_id *devices)
    : _ecl_program(context, num_devices, devices),  m_job_instance_templates(num_devices) {
        m_stack_size = T::DEFAULT_STACK_SIZE;
    }

    virtual int getPlatform() const {
        return T::EclPlId;
    }

    std::vector<typename T::job_instance> m_job_instance_templates;
    std::vector<std::shared_ptr<typename T::Job>> m_jobs;
    std::shared_ptr<typename T::ELF> m_elf;
};

#define ECL_KERNEL_ARG_TYPE_BASIC (0 << 28)
#define ECL_KERNEL_ARG_TYPE_GLOBAL (1 << 28)
#define ECL_KERNEL_ARG_TYPE_LOCAL (2 << 28)
#define ECL_KERNEL_ARG_TYPE_DMA (3 << 28)
#define ECL_KERNEL_ARG_TYPE_NC_GLOBAL (4 << 28)

struct _ecl_kernel : public _ecl_object {
    _ecl_kernel(ecl_program program, const char *kernel_name, uint32_t kernel_addr)
        : m_program(program), m_kernel_name(kernel_name), m_kernel_addr(kernel_addr) {}
    explicit _ecl_kernel(ecl_kernel src_kernel)
        : m_kernel_addr(src_kernel->m_kernel_addr),
          m_program(src_kernel->m_program),
          m_kernel_name(src_kernel->m_kernel_name) {
        std::unique_lock<std::mutex> lock(src_kernel->m_kernel_guard);
        m_args = src_kernel->m_args;
    }

    ~_ecl_kernel() {}

    const uint32_t m_kernel_addr;
    const boost::intrusive_ptr<_ecl_program> m_program;
    const std::string m_kernel_name;

    std::mutex m_kernel_guard;
    std::map<ecl_uint, std::pair<ecl_uint, std::vector<uint8_t>>> m_args;
};

struct _ecl_event : public _ecl_object {
    _ecl_event(ecl_context context, ecl_command_queue queue, ecl_command_type command_type)
        : m_context(context),
          m_queue(queue),
          m_command_type(command_type),
          m_command_status(ECL_QUEUED) {}
    ~_ecl_event() {}

    void set_command_status(ecl_int command_status) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (command_status <= ECL_COMPLETE && m_command_status <= ECL_COMPLETE)
                throw std::runtime_error("trying to complere command that already completed");
            if (command_status >= m_command_status)
                throw std::runtime_error("command status should decrease");
            add_ref();
            m_command_status = command_status;
            // Negative statuses indicate an error during execution. Completion callbacks must be
            // called in this case.
            if (command_status < ECL_COMPLETE) command_status = ECL_COMPLETE;
            for (auto it = m_callbacks[command_status].cbegin();
                 it != m_callbacks[command_status].cend(); ++it) {
                lock.unlock();
                it->first(this, m_command_status, it->second);
                lock.lock();
            }
            if (m_command_status <= ECL_COMPLETE) m_status_cond.notify_all();
        }
        release();
    }
    ecl_int get_command_status() {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_command_status;
    }

    const boost::intrusive_ptr<_ecl_context> m_context;
    const boost::intrusive_ptr<_ecl_command_queue> m_queue;
    const ecl_command_type m_command_type;

    std::condition_variable m_status_cond;
    std::mutex m_mutex;
    ecl_int m_command_status;

    std::list<std::pair<void(ECL_CALLBACK *)(ecl_event, ecl_int, void *), void *>> m_callbacks[4];
};

#define ECL_RETURN_GETINFO_INNER(__SIZE__, MEMASSIGN)               \
    do {                                                               \
        if (param_value) {                                             \
            if (param_value_size < __SIZE__) return ECL_INVALID_VALUE; \
            MEMASSIGN;                                                 \
        }                                                              \
        if (param_value_size_ret) *param_value_size_ret = __SIZE__;    \
        return ECL_SUCCESS;                                            \
    } while (0)

#define ECL_RETURN_GETINFO_SIZE(__SIZE__, __POINTER__) \
    ECL_RETURN_GETINFO_INNER(__SIZE__, memcpy(param_value, __POINTER__, __SIZE__))

#define ECL_RETURN_GETINFO_STR(__STR__)                                                 \
    do {                                                                                   \
        size_t const value_size = strlen(__STR__) + 1;                                     \
        ECL_RETURN_GETINFO_INNER(value_size, memcpy(param_value, __STR__, value_size)); \
    } while (0)

#define ECL_RETURN_GETINFO(__TYPE__, __VALUE__)                                     \
    do {                                                                               \
        size_t const value_size = sizeof(__TYPE__);                                    \
        ECL_RETURN_GETINFO_INNER(value_size, *(__TYPE__ *)param_value = __VALUE__); \
    } while (0)

#define ECL_RETURN_GETINFO_ARRAY(__TYPE__, __NUM__, __VALUE__)                            \
    do {                                                                                     \
        size_t const value_size = __NUM__ * sizeof(__TYPE__);                                \
        ECL_RETURN_GETINFO_INNER(value_size, memcpy(param_value, __VALUE__, value_size)); \
    } while (0)
