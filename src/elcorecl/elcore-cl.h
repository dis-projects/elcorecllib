// Copyright 2018 RnD Center "ELVEES", JSC

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
#endif

#include "libelcore.h"

#include <linux/dma-buf.h>

#define E50_DEFAULT_STACK_SIZE (2048 * 1024)

struct _ecl_platform_id {
    unsigned long id;
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

#ifdef EMULATE_DEVICES
struct _ecl_device {
    _ecl_device(const std::string &) {}
    ~_ecl_device() {}
    int enqueue_job(elcore50_job_instance *) { return 0; }
    int get_job_status(elcore50_job_instance_status *job_status) {
        job_status->state = ELCORE50_JOB_STATUS_DONE;
        job_status->error = ELCORE50_JOB_STATUS_SUCCESS;
        return 0;
    }
    void poll_job(int job_fd) {}
    int close_job(int job_fd) { return 0; }
};
#else
struct _ecl_device {
    _ecl_device(const std::string &dev_name) {
        m_fd = open(dev_name.c_str(), O_RDONLY, 0);
        if (m_fd == -1) throw std::runtime_error("can't open device");
        struct elcore50_device_info info;
        ioctl(m_fd, ELCORE50_IOC_GET_CORE_IDX, &info);
        m_devnum = info.cluster_id * info.cluster_cap + info.core_in_cluster_id;
    }
    ~_ecl_device() { close(); }
    int create_job(elcore50_job *job) {
        return ioctl(m_fd, ELCORE50_IOC_CREATE_JOB, job);
    }
    int enqueue_job(elcore50_job_instance *job) {
        return ioctl(m_fd, ELCORE50_IOC_ENQUEUE_JOB, job);
    }
    int get_job_status(elcore50_job_instance_status *job_status) {
        int ret = ioctl(m_fd, ELCORE50_IOC_GET_JOB_STATUS, job_status);
        if (ret) throw std::runtime_error("ELCORE50_IOC_GET_JOB_STATUS failed");
        return ret;
    }
    int create_buffer(struct elcore50_buf *buf) {
        int ret = ioctl(m_fd, ELCORE50_IOC_CREATE_BUFFER, buf);
        if (ret) throw std::runtime_error("ELCORE50_IOC_CREATE_BUFFER failed");
        return ret;
    }
    int create_mapper(struct elcore50_buf *buf) {
        int ret = ioctl(m_fd, ELCORE50_IOC_CREATE_MAPPER, buf);
        if (ret) throw std::runtime_error("ELCORE50_IOC_CREATE_MAPPER failed");
        return ret;
    }
    int sync_for_host(struct elcore50_buf_sync *sync) {
        sync->dir = ELCORE50_BUF_SYNC_DIR_TO_CPU;
        int ret = ioctl(m_fd, ELCORE50_IOC_SYNC_BUFFER, sync);
        if (ret) throw std::runtime_error("ELCORE50_IOC_SYNC_BUFFER failed");
        return ret;
    }
    int sync_for_device(struct elcore50_buf_sync *sync) {
        sync->dir = ELCORE50_BUF_SYNC_DIR_TO_DEVICE;
        int ret = ioctl(m_fd, ELCORE50_IOC_SYNC_BUFFER, sync);
        if (ret) throw std::runtime_error("ELCORE50_IOC_SYNC_BUFFER failed");
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

struct _ecl_device_id {
    _ecl_device_id(const std::string &dev_name) : m_dev_name(dev_name), m_dev(nullptr) {}

    std::shared_ptr<_ecl_device> open_device() {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_dev == nullptr)
            m_dev = std::shared_ptr<_ecl_device>(new _ecl_device(m_dev_name));
        return m_dev;
    }

 private:
    const std::string m_dev_name;
    std::shared_ptr<_ecl_device> m_dev;
    std::mutex m_mutex;
};

bool IsDeviceValid(ecl_device_id device_id);

struct _ecl_context : public _ecl_object {
    _ecl_context(std::vector<_ecl_device_id *> &&device_ids,
                 void(ECL_CALLBACK *pfn_notify)(const char *, const void *, size_t, void *),
                 void *user_data)
        : m_device_ids(device_ids), m_pfn_notify(pfn_notify), m_user_data(user_data) {
        for (_ecl_device_id *device_id : m_device_ids)
            m_devices.push_back(device_id->open_device());
    }

    std::vector<_ecl_device_id *> m_device_ids;
    std::vector<std::shared_ptr<_ecl_device>> m_devices;

 private:
    void(ECL_CALLBACK *m_pfn_notify)(const char *, const void *, size_t, void *);
    void *m_user_data;
};

struct _ecl_kernel_instance : _ecl_object {
    elcore50_job_instance m_job_instance;
    elcore50_job_instance_status m_job_status;
};

struct _ecl_sync_object_instance : _ecl_object {
    ecl_mem m_memobj;
    ecl_map_flags m_map_flags;
    size_t m_offset;
    size_t m_cb;
};

struct ecl_command_queue_node : _ecl_object {
    boost::intrusive_ptr<_ecl_event> m_event;
    std::vector<boost::intrusive_ptr<_ecl_event>> m_wait_list;
    boost::intrusive_ptr<_ecl_command_queue> m_queue;
    boost::intrusive_ptr<_ecl_kernel_instance> m_kernel;
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
        m_device = m_device_id->open_device();

        std::thread t(&_ecl_command_queue::PollThread, this);
        t.detach();
    }
    ~_ecl_command_queue() {}

    _ecl_device_id *const m_device_id;
    const boost::intrusive_ptr<_ecl_context> m_context;
    const ecl_command_queue_properties m_properties;

    void PollThread();

    virtual int release() {
        int count = _ecl_object::release();
        if (count != 0) m_commands_list_cond.notify_all();
        return count;
    }
    std::shared_ptr<_ecl_device> m_device;

    std::mutex m_commands_list_guard;
    std::condition_variable m_commands_list_cond;
    std::list<boost::intrusive_ptr<ecl_command_queue_node>> m_submitted_kernels_list;
    boost::intrusive_ptr<ecl_command_queue_node> m_last_command_in_queue;
};

struct _ecl_mem : public _ecl_object {
    _ecl_mem(ecl_context context, ecl_mem_flags flags, size_t size, void *host_ptr,
             ecl_mem_object_type type, int dma_buf_fd = -1)
        : m_context(context), m_flags(flags), m_size(size), m_host_ptr(host_ptr), m_type(type) {
        m_mem_info.size = m_size;
        m_dev = m_context->m_devices[0];
        if (flags & ECL_MEM_USE_DMABUF) {
            m_mem_info.type = ELCORE50_NONCACHED_BUFFER;
            m_mem_info.dmabuf_fd = dma_buf_fd;
            m_host_ptr = mmap(NULL, m_size, PROT_READ | PROT_WRITE, MAP_SHARED,
                              m_mem_info.dmabuf_fd, 0);
            if (m_host_ptr == MAP_FAILED)
                m_host_ptr = nullptr;
        } else {
            m_mem_info.mapper_fd = m_mem_info.dmabuf_fd = 0;
            m_mem_info.type = ELCORE50_CACHED_BUFFER_FROM_UPTR;
            m_mem_info.p = reinterpret_cast<__u64>(m_host_ptr);
            m_dev->create_buffer(&m_mem_info);
        }

        m_dev->create_mapper(&m_mem_info);
        m_map_flags = 0;
    }
    ~_ecl_mem() {
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
    int sync_for_host(size_t offset, size_t cb, ecl_map_flags map_flags) {
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
    int sync_for_device() {
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

    const boost::intrusive_ptr<_ecl_context> m_context;
    const ecl_mem_object_type m_type;
    const ecl_mem_flags m_flags;
    ecl_map_flags m_map_flags;
    const size_t m_size;
    void *m_host_ptr;
    struct elcore50_buf m_mem_info;
    struct elcore50_buf_sync m_mem_sync;
    std::list<std::pair<void(ECL_CALLBACK *)(ecl_mem, void *), void *>> m_destructor_callbacks;
    std::shared_ptr<_ecl_device> m_dev;
};

struct _ecl_program : public _ecl_object {
    _ecl_program(ecl_context context, ecl_uint num_devices, const ecl_device_id *devices)
        : m_context(context),
          m_job_instance_templates(num_devices),
          m_devices(devices, devices + num_devices),
          m_stack_size(E50_DEFAULT_STACK_SIZE) {
        m_build_status = ECL_BUILD_NONE;
    }
    ~_ecl_program() {
        for (auto it = m_release_callbacks.crbegin(); it != m_release_callbacks.crend(); ++it) {
            it->first(this, it->second);
        }
    }

    const size_t get_stack_size() { return m_stack_size; }

    const boost::intrusive_ptr<_ecl_context> m_context;
    std::list<std::pair<void(ECL_CALLBACK *)(ecl_program, void *), void *>> m_release_callbacks;

    std::vector<elcore50_job_instance> m_job_instance_templates;

    std::vector<_ecl_device_id *> m_devices;
    ecl_build_status m_build_status;
    std::vector<std::shared_ptr<ElcoreJob>> m_jobs;
    std::shared_ptr<ElcoreELF> m_elf;
    std::vector<boost::intrusive_ptr<_ecl_mem>> m_shared_sections;
    size_t m_stack_size;
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
