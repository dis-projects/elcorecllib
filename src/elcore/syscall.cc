// Copyright 2021 RnD Center "ELVEES", JSC

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>

#ifdef DRIVER_NOT_INSTALLED
#include <elcore50.h>
#else
#include <linux/elcore50.h>
#endif
#include "syscall.h"

extern char **environ;

namespace {
class SyscallMetaData {
 public:
    SyscallMetaData() {
        struct elcore50_device_info info;

        char *env = std::getenv("ELCORE_WRITE_CORENUM");
        if (env) {
            if (!strcmp(env, "1"))
                 m_corenum_write = true;
             else if (!strcmp(env, "0"))
                 m_corenum_write = false;
            else
                 throw std::runtime_error("possible ELCORE_WRITE_CORENUM values: 0, 1");
        }

        env = std::getenv("ELCORE_WRITE_TIMESTAMP");
        if (env) {
            if (!strcmp(env, "1"))
                m_timestamp_write = true;
            else if (!strcmp(env, "0"))
                m_timestamp_write = false;
            else
                throw std::runtime_error("possible ELCORE_WRITE_TIMESTAMP values: 0, 1");
        }

        int elcore_fd = open("/dev/elcore0", O_RDWR, 0);
        ioctl(elcore_fd, ELCORE50_IOC_GET_CORE_IDX, &info);
        m_stdout_buffers.resize(info.nclusters * info.cluster_cap);
        m_stderr_buffers.resize(info.nclusters * info.cluster_cap);
    }

    bool m_corenum_write;
    bool m_timestamp_write;
    std::vector<std::string> m_stdout_buffers;
    std::vector<std::string> m_stderr_buffers;
} s_data;
}

int get_elcore_num(int elcore_fd) {
    struct elcore50_device_info info;
    ioctl(elcore_fd, ELCORE50_IOC_GET_CORE_IDX, &info);
    return info.cluster_id * info.cluster_cap + info.core_in_cluster_id;
}

void ElcoreFlushPreamble(int elcore_fd) {
    static std::mutex write_guard;
    int elcore_id = get_elcore_num(elcore_fd);
    std::string &stderr_buffer = s_data.m_stderr_buffers[elcore_id];
    std::string &stdout_buffer = s_data.m_stdout_buffers[elcore_id];

    std::unique_lock<std::mutex> lock(write_guard);

    if (!stdout_buffer.empty()) {
        stdout_buffer += "\n";
        write(1, stdout_buffer.c_str(), stdout_buffer.length());
    }

    if (!stderr_buffer.empty()) {
        stderr_buffer += "\n";
        write(2, stderr_buffer.c_str(), stderr_buffer.length());
    }
}

std::string get_preamble(int device_fd) {
    char buf_timestamp[21], buf_devnum[11];
    std::string preamble;

    if (s_data.m_timestamp_write) {
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(buf_timestamp, sizeof(buf_timestamp), "%d-%m-%Y %H:%M:%S:", timeinfo);
        preamble += std::string(buf_timestamp) + " ";
    }

    if (s_data.m_corenum_write) {
        snprintf(buf_devnum, sizeof(buf_devnum), "elcore%2d: ", get_elcore_num(device_fd));
        preamble += std::string(buf_devnum);
    }
    return preamble;
}

int write_with_preamble_buffered(const char *msg, int fd, const int size, int device_fd) {
    static std::mutex write_guard;
    std::string preamble = get_preamble(device_fd);
    bool is_terminated = true;
    int elcore_id = get_elcore_num(device_fd);
    std::string &buffer = (fd == 1) ? s_data.m_stdout_buffers[elcore_id] :
                                      s_data.m_stderr_buffers[elcore_id];

    if (buffer.empty())
        buffer = preamble;

    std::stringstream lines;
    lines.write(msg, size);

    size_t n_lines = std::count(msg, msg + size, '\n');
    if (n_lines == 0) {
        buffer += lines.str();
        return size;
    }

    std::string line;
    int line_idx = 0;
    while (std::getline(lines, line, '\n')) {
        if (line_idx == n_lines) break;  // if the last line without '\n'

        if (line_idx != 0)
            buffer += preamble;
        buffer += line + "\n";

        line_idx++;
    }

    // Output completed lines
    std::unique_lock<std::mutex> lock(write_guard);
    write(fd, buffer.c_str(), buffer.length());

    buffer.clear();
    if (line.length())
        buffer = preamble + line;
    return size;
}

void ElcoreSyscallHandler(int job_instance_fd, int job_fd) {
    struct elcore50_message message;
    int ret = read(job_instance_fd, &message, sizeof(struct elcore50_message));
    if (ret < 0) {
        printf("Error: read job file failed with %d\n", errno);
        return;
    }

    struct stat ret_stat;
    struct tms ret_tms;
    struct stat_compat *stat_compat;
    struct tms_compat *tms_compat;
    char *el50_env, *env_buf = NULL;
    void *tmp = NULL;
    uint32_t *el50_env_size, full_needed_size;
    int flags;
    switch (message.num) {
    case SC_GETTIMEOFDAY:
        message.retval = gettimeofday((struct timeval *)message.arg0,
                                      (struct timezone *) message.arg1);
        break;
    case SC_WRITE:
        if ((s_data.m_timestamp_write || s_data.m_corenum_write) && (message.arg0 <= 2))
            ret = write_with_preamble_buffered(
                      (char *) message.arg1, (int) message.arg0,
                      message.arg2, job_fd);
        else
            ret = write((int) message.arg0, (char *) message.arg1, message.arg2);
        break;
    case SC_READ:
        ret = read((int) message.arg0, (char *) message.arg1, message.arg2);
        break;
    case SC_OPEN:
        flags = 0;
        if (message.arg1 & O_CREAT_COMPAT) flags |= O_CREAT;
        if (message.arg1 & O_EXCL_COMPAT) flags |= O_EXCL;
        if (message.arg1 & O_NOCTTY_COMPAT) flags |= O_NOCTTY;
        if (message.arg1 & O_TRUNC_COMPAT) flags |= O_TRUNC;
        if (message.arg1 & O_APPEND_COMPAT) flags |= O_APPEND;
        if (message.arg1 & O_NONBLOCK_COMPAT) flags |= O_NONBLOCK;
        if (message.arg1 & O_SYNC_COMPAT) flags |= O_SYNC;
        if (message.arg1 & O_RDWR_COMPAT) flags |= O_RDWR;
        if (message.arg1 & O_RDONLY_COMPAT) flags |= O_RDONLY;
        if (message.arg1 & O_WRONLY_COMPAT) flags |= O_WRONLY;
        ret = open((char *) message.arg0, flags, message.arg2);
        break;
    case SC_CLOSE:
        if (message.arg0 > 2)
            ret = close((int) message.arg0);
        else {
            ret = -1;
            errno = EINVAL;
        }
        break;
    case SC_FSTAT:
        ret = fstat((int)message.arg0, &ret_stat);
        stat_compat =  reinterpret_cast<struct stat_compat *>(message.arg1);
        stat_compat->dev = ret_stat.st_dev;
        stat_compat->ino = ret_stat.st_ino;
        stat_compat->mode = ret_stat.st_mode;
        stat_compat->nlink = ret_stat.st_nlink;
        stat_compat->uid = ret_stat.st_uid;
        stat_compat->gid = ret_stat.st_gid;
        stat_compat->rdev = ret_stat.st_rdev;
        stat_compat->size = ret_stat.st_size;
        stat_compat->blksize = ret_stat.st_blksize;
        stat_compat->blocks = ret_stat.st_blocks;
        stat_compat->atime = ret_stat.st_atime;
        stat_compat->mtime = ret_stat.st_mtime;
        stat_compat->ctime = ret_stat.st_ctime;
        break;
    case SC_LSEEK:
        ret = lseek((int)message.arg0, (int) message.arg1, (int)message.arg2);
        break;
    case SC_ISATTY:
        ret = isatty((int) message.arg0);
        break;
    case SC_STAT:
        ret = stat((const char *)message.arg0, &ret_stat);
        stat_compat =  reinterpret_cast<struct stat_compat *>(message.arg1);
        stat_compat->dev = ret_stat.st_dev;
        stat_compat->ino = ret_stat.st_ino;
        stat_compat->mode = ret_stat.st_mode;
        stat_compat->nlink = ret_stat.st_nlink;
        stat_compat->uid = ret_stat.st_uid;
        stat_compat->gid = ret_stat.st_gid;
        stat_compat->rdev = ret_stat.st_rdev;
        stat_compat->size = ret_stat.st_size;
        stat_compat->blksize = ret_stat.st_blksize;
        stat_compat->blocks = ret_stat.st_blocks;
        stat_compat->atime = ret_stat.st_atime;
        stat_compat->mtime = ret_stat.st_mtime;
        stat_compat->ctime = ret_stat.st_ctime;
        break;
    case SC_LINK:
        ret = link((char *)message.arg0, (char *)message.arg1);
        break;
    case SC_UNLINK:
        ret = unlink((char *)message.arg0);
        break;
    case SC_CHDIR:
        ret = chdir((char *)message.arg0);
        break;
    case SC_TIMES:
        ret = times(&ret_tms);
        tms_compat = reinterpret_cast<struct tms_compat *>(message.arg0);
        tms_compat->tms_utime = ret_tms.tms_utime;
        tms_compat->tms_stime = ret_tms.tms_stime;
        tms_compat->tms_cutime = ret_tms.tms_cutime;
        tms_compat->tms_cstime = ret_tms.tms_cstime;
        break;
    case SC_GET_ENV:
        el50_env = reinterpret_cast<char *>(message.arg0);
        el50_env_size = reinterpret_cast<uint32_t *>(message.arg1);
        if (!el50_env_size) { ret = -1; errno = EINVAL; break; }

        full_needed_size = 0; env_buf = NULL;
        for (char **host_env = environ; *host_env; host_env++) {
            ret = full_needed_size;
            full_needed_size += strlen(*host_env);
            tmp = realloc(env_buf, full_needed_size + 1);
            if (!tmp)  { ret = -1; errno = EINVAL; break; }
            env_buf = (char *) tmp;
            strcpy(env_buf + ret, *host_env);
            *strchr(env_buf + ret, '=') = '\0';  // replace the first "="
            // Insert env variables separator
            env_buf[full_needed_size] = '\0';
            full_needed_size += 1;
        }
        if (ret == -1) break;
        // Insert '\0' separator for end of line;
        full_needed_size += 1;
        tmp = realloc(env_buf, full_needed_size);
        if (!tmp)  { ret = -1; errno = EINVAL; break; }
        env_buf = (char *) tmp;
        env_buf[full_needed_size - 1] = '\0';

        if (el50_env) memcpy(el50_env, env_buf, std::min(*el50_env_size, full_needed_size));
        *el50_env_size = full_needed_size;
        ret = 0;
        break;
    default:
        throw std::runtime_error("incorrect syscall number");
        break;
    }
    free(env_buf);

    // TODO: errno mapping #SOLARISSW-901
    message.retval = (ret < 0) ? -errno : ret;
    message.type = ELCORE50_MESSAGE_SYSCALL_REPLY;
    ret = write(job_instance_fd, &message, sizeof(struct elcore50_message));
    if (ret < 0) {
        printf("Error: write job file failed with %d\n", errno);
        return;
    }
}
