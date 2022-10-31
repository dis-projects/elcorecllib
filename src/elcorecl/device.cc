// Copyright 2018 RnD Center "ELVEES", JSC

#include <list>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "elcore-cl.h"

namespace {
#ifdef EMULATE_DEVICES
class DeviceListInitializer {
 public:
    DeviceListInitializer() {
        for (int i = 0; i < 16; ++i) {
        devices.push_back(new _ecl_device_id(std::to_string(i)));
        }
    }
    ~DeviceListInitializer() {
        for (auto it = devices.begin(); it != devices.end(); ++it) {
            delete *it;
        }
    }
    std::list<ecl_device_id> devices;
};
#else
class DeviceListInitializer {
 public:
    DeviceListInitializer() {
        const std::string target_path("/dev/");
        const boost::regex filter_elcore50("elcore\\d+");

        boost::filesystem::directory_iterator end_itr;
        for (boost::filesystem::directory_iterator i(target_path); i != end_itr; ++i) {
            // Skip if not a file
            if (!boost::filesystem::is_other(i->status())) continue;

            if (!boost::regex_match(i->path().filename().string(), filter_elcore50)) continue;

            // File matches, store it
            int m_fd = open(i->path().string().c_str(), O_RDONLY, 0);
            if (m_fd == -1) throw std::runtime_error("can't open device");

            struct elcore50_device_info device_info;
            ioctl(m_fd, ELCORE50_IOC_GET_CORE_IDX, &device_info);
            close(m_fd);

            int device_idx = device_info.cluster_id * device_info.cluster_cap +
                                     device_info.core_in_cluster_id;

            if (devices.size() <= device_idx) devices.resize(device_idx + 1, nullptr);

            devices[device_idx] = new _ecl_device_id(i->path().string());
        }

        for (auto it = devices.begin(); it != devices.end(); ++it)
            if (*it == nullptr) throw std::runtime_error("failed to get devices");
    }
    ~DeviceListInitializer() {
        for (auto it = devices.begin(); it != devices.end(); ++it) {
            delete *it;
        }
    }
    std::vector<ecl_device_id> devices;
};
#endif

DeviceListInitializer &DeviceList() {
    static DeviceListInitializer s_dev_list;
    return s_dev_list;
}

}

bool IsDeviceValid(ecl_device_id device_id) {
    return std::find(DeviceList().devices.cbegin(), DeviceList().devices.cend(), device_id) !=
           DeviceList().devices.cend();
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetDeviceIDs(ecl_platform_id platform, ecl_device_type device_type, ecl_uint num_entries,
                ecl_device_id* devices, ecl_uint* num_devices) ECL_API_SUFFIX__VERSION_1_0 {
    ecl_platform_id tmp_platform;

    if (platform == nullptr) return ECL_INVALID_PLATFORM;

    if (num_entries == 0 && devices != nullptr) return ECL_INVALID_VALUE;
    if (num_devices == nullptr && devices == nullptr) return ECL_INVALID_VALUE;

    eclGetPlatformIDs(1, &tmp_platform, nullptr);
    if (platform != tmp_platform) return ECL_INVALID_PLATFORM;

    if (!(device_type & ECL_DEVICE_TYPE_CUSTOM)) return ECL_INVALID_DEVICE_TYPE;

    if (DeviceList().devices.size() == 0) return ECL_DEVICE_NOT_FOUND;

    if (num_devices != nullptr) *num_devices = DeviceList().devices.size();

    ecl_uint devices_added = 0;

    if (devices != nullptr) {
        for (auto&& device : DeviceList().devices) {
            if (devices_added >= num_entries) break;
            devices[devices_added++] = device;
        }
    }

    if (devices_added > 0 || num_entries == 0)
        return ECL_SUCCESS;
    else
        return ECL_DEVICE_NOT_FOUND;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetDeviceInfo(ecl_device_id device, ecl_device_info param_name, size_t param_value_size,
                 void* param_value, size_t* param_value_size_ret) ECL_API_SUFFIX__VERSION_1_0 {
    if (!IsDeviceValid(device)) return ECL_INVALID_DEVICE;

    switch (param_name) {
        case ECL_DEVICE_IMAGE_SUPPORT:
            ECL_RETURN_GETINFO(ecl_bool, false);
        case ECL_DEVICE_TYPE:
            ECL_RETURN_GETINFO(ecl_device_type, ECL_DEVICE_TYPE_CUSTOM);
        case ECL_DEVICE_VENDOR_ID:
            ECL_RETURN_GETINFO(ecl_uint, 0xE50);
        case ECL_DEVICE_MAX_COMPUTE_UNITS:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:
            ECL_RETURN_GETINFO(ecl_uint, 3);
        case ECL_DEVICE_MAX_WORK_GROUP_SIZE:
            ECL_RETURN_GETINFO(size_t, 1);
        case ECL_DEVICE_MAX_WORK_ITEM_SIZES: {
            /* We allocate a 3-elementa array for this in pthread.c */
            typedef struct {
                size_t size[3];
            } size_t_3;
            size_t_3 max_work_item_sizes = {1, 1, 1};
            ECL_RETURN_GETINFO(size_t_3, max_work_item_sizes);
        }
        case ECL_DEVICE_MAX_MEM_ALLOC_SIZE:
            ECL_RETURN_GETINFO(ecl_ulong, 0xffffffff);
        case ECL_DEVICE_MAX_DMAMEM_ALLOC_SIZE:
            ECL_RETURN_GETINFO(ecl_ulong, 0x3fffffffff);
        case ECL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_PREFERRED_VECTOR_WIDTH_INT:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_MAX_CLOCK_FREQUENCY:
            ECL_RETURN_GETINFO(ecl_uint, 1000);
        case ECL_DEVICE_ADDRESS_BITS:
            ECL_RETURN_GETINFO(ecl_uint, 32);
        case ECL_DEVICE_MAX_READ_IMAGE_ARGS:
            ECL_RETURN_GETINFO(ecl_uint, 0);
        case ECL_DEVICE_MAX_WRITE_IMAGE_ARGS:
            ECL_RETURN_GETINFO(ecl_uint, 0);
        case ECL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS:
            ECL_RETURN_GETINFO(ecl_uint, 0);
        case ECL_DEVICE_IMAGE2D_MAX_WIDTH:
            ECL_RETURN_GETINFO(size_t, 0);
        case ECL_DEVICE_IMAGE2D_MAX_HEIGHT:
            ECL_RETURN_GETINFO(size_t, 0);
        case ECL_DEVICE_IMAGE3D_MAX_WIDTH:
            ECL_RETURN_GETINFO(size_t, 0);
        case ECL_DEVICE_IMAGE3D_MAX_HEIGHT:
            ECL_RETURN_GETINFO(size_t, 0);
        case ECL_DEVICE_IMAGE3D_MAX_DEPTH:
            ECL_RETURN_GETINFO(size_t, 0);
        case ECL_DEVICE_IMAGE_MAX_BUFFER_SIZE:
            ECL_RETURN_GETINFO(size_t, 0);
        case ECL_DEVICE_IMAGE_MAX_ARRAY_SIZE:
            ECL_RETURN_GETINFO(size_t, 0);
        case ECL_DEVICE_MAX_SAMPLERS:
            ECL_RETURN_GETINFO(ecl_uint, 0);
        case ECL_DEVICE_MAX_PARAMETER_SIZE:
            ECL_RETURN_GETINFO(size_t, 256 * 1024);
        case ECL_DEVICE_MEM_BASE_ADDR_ALIGN:
            ECL_RETURN_GETINFO(ecl_uint, 12);
        case ECL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_SINGLE_FP_CONFIG:
            ECL_RETURN_GETINFO(ecl_ulong, ECL_FP_DENORM | ECL_FP_INF_NAN);
        case ECL_DEVICE_GLOBAL_MEM_CACHE_TYPE:
            ECL_RETURN_GETINFO(ecl_uint, ECL_READ_WRITE_CACHE);
        case ECL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE:
            ECL_RETURN_GETINFO(ecl_uint, 1024);
        case ECL_DEVICE_GLOBAL_MEM_CACHE_SIZE:
            ECL_RETURN_GETINFO(ecl_ulong, 256 * 1024);
        case ECL_DEVICE_GLOBAL_MEM_SIZE:
            ECL_RETURN_GETINFO(ecl_ulong, 0xffffffff);
        case ECL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:
            ECL_RETURN_GETINFO(ecl_ulong, 0xffffffff);
        case ECL_DEVICE_MAX_CONSTANT_ARGS:
            ECL_RETURN_GETINFO(ecl_uint, 1024);
        case ECL_DEVICE_LOCAL_MEM_TYPE:
            ECL_RETURN_GETINFO(ecl_uint, ECL_LOCAL);
        case ECL_DEVICE_LOCAL_MEM_SIZE:
            ECL_RETURN_GETINFO(ecl_ulong, 256 * 1024);
        case ECL_DEVICE_ERROR_CORRECTION_SUPPORT:
            ECL_RETURN_GETINFO(ecl_bool, ECL_FALSE);
        case ECL_DEVICE_PROFILING_TIMER_RESOLUTION:
            ECL_RETURN_GETINFO(size_t, 1000000);
        case ECL_DEVICE_ENDIAN_LITTLE:
            ECL_RETURN_GETINFO(ecl_uint, ECL_TRUE);
        case ECL_DEVICE_AVAILABLE:
            ECL_RETURN_GETINFO(ecl_bool, ECL_TRUE);
        case ECL_DEVICE_COMPILER_AVAILABLE:
            ECL_RETURN_GETINFO(ecl_bool, ECL_FALSE);
        case ECL_DEVICE_LINKER_AVAILABLE:
            ECL_RETURN_GETINFO(ecl_bool, ECL_FALSE);
        case ECL_DEVICE_EXECUTION_CAPABILITIES:
            ECL_RETURN_GETINFO(ecl_device_exec_capabilities, ECL_EXEC_NATIVE_KERNEL);
        case ECL_DEVICE_NAME:
            ECL_RETURN_GETINFO_STR("Elcore50");
        case ECL_DEVICE_VENDOR:
            ECL_RETURN_GETINFO_STR("RnD Center \"ELVEES\", JSC");
        case ECL_DRIVER_VERSION:
            ECL_RETURN_GETINFO_STR("1.0");
        case ECL_DEVICE_PROFILE:
            ECL_RETURN_GETINFO_STR("EMBEDDED_PROFILE");
        case ECL_DEVICE_VERSION:
            ECL_RETURN_GETINFO_STR("50.0");
        case ECL_DEVICE_EXTENSIONS:
            ECL_RETURN_GETINFO_STR("ecl_khr_fp64");
        case ECL_DEVICE_PLATFORM: {
            /* Return the first platform id, assuming this is the only
               platform id (which is currently always the case for pocl) */
            ecl_platform_id platform_id;
            eclGetPlatformIDs(1, &platform_id, nullptr);
            ECL_RETURN_GETINFO(ecl_platform_id, platform_id);
        }
        case ECL_DEVICE_DOUBLE_FP_CONFIG:
            ECL_RETURN_GETINFO(ecl_ulong, 0);
        case ECL_DEVICE_HALF_FP_CONFIG:
            ECL_RETURN_GETINFO(ecl_ulong, 0);
        case ECL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF:
            ECL_RETURN_GETINFO(ecl_uint, 0);
        case ECL_DEVICE_HOST_UNIFIED_MEMORY:
            ECL_RETURN_GETINFO(ecl_bool, ECL_TRUE);
        case ECL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_NATIVE_VECTOR_WIDTH_INT:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_NATIVE_VECTOR_WIDTH_LONG:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE:
            ECL_RETURN_GETINFO(ecl_uint, 0);
        case ECL_DEVICE_NATIVE_VECTOR_WIDTH_HALF:
            ECL_RETURN_GETINFO(ecl_uint, 0);
        case ECL_DEVICE_OPENCL_C_VERSION:
            ECL_RETURN_GETINFO_STR("none");
        case ECL_DEVICE_BUILT_IN_KERNELS:
            ECL_RETURN_GETINFO_STR("");
        case ECL_DEVICE_PARENT_DEVICE:
            ECL_RETURN_GETINFO(ecl_device_id, nullptr);
        case ECL_DEVICE_PARTITION_MAX_SUB_DEVICES:
            ECL_RETURN_GETINFO(ecl_uint, 0);
        case ECL_DEVICE_PARTITION_PROPERTIES:
            ECL_RETURN_GETINFO(ecl_device_partition_property, 0);
        case ECL_DEVICE_PARTITION_TYPE:
            ECL_RETURN_GETINFO(ecl_device_partition_property, 0);
        case ECL_DEVICE_PARTITION_AFFINITY_DOMAIN:
            ECL_RETURN_GETINFO(ecl_device_affinity_domain, 0);
        case ECL_DEVICE_PREFERRED_INTEROP_USER_SYNC:
            ECL_RETURN_GETINFO(ecl_bool, ECL_TRUE);
        case ECL_DEVICE_PRINTF_BUFFER_SIZE:
            ECL_RETURN_GETINFO(size_t, 1024 * 1024);
        case ECL_DEVICE_REFERENCE_COUNT:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_SVM_CAPABILITIES:
            ECL_RETURN_GETINFO(ecl_device_svm_capabilities, ECL_DEVICE_SVM_FINE_GRAIN_SYSTEM);
        case ECL_DEVICE_MAX_ON_DEVICE_EVENTS:
            ECL_RETURN_GETINFO(ecl_uint, 1024);
        case ECL_DEVICE_MAX_ON_DEVICE_QUEUES:
            ECL_RETURN_GETINFO(ecl_uint, 1);
        case ECL_DEVICE_MAX_PIPE_ARGS:
            ECL_RETURN_GETINFO(ecl_uint, 16);
        case ECL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS:
            ECL_RETURN_GETINFO(ecl_uint, 16);
        case ECL_DEVICE_PIPE_MAX_PACKET_SIZE:
            ECL_RETURN_GETINFO(ecl_uint, 1024);
        case ECL_DEVICE_QUEUE_ON_DEVICE_PREFERRED_SIZE:
            ECL_RETURN_GETINFO(ecl_uint, 0);
        case ECL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE:
            ECL_RETURN_GETINFO(ecl_uint, 0);
        case ECL_DEVICE_PREFERRED_GLOBAL_ATOMIC_ALIGNMENT:
            ECL_RETURN_GETINFO(ecl_uint, 4);
        case ECL_DEVICE_PREFERRED_LOCAL_ATOMIC_ALIGNMENT:
            ECL_RETURN_GETINFO(ecl_uint, 4);
        case ECL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT:
            ECL_RETURN_GETINFO(ecl_uint, 4);
        case ECL_DEVICE_SPIR_VERSIONS:
            ECL_RETURN_GETINFO_STR("");
    }
    return ECL_INVALID_VALUE;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclCreateSubDevices(ecl_device_id in_device, const ecl_device_partition_property* properties,
                    ecl_uint num_devices, ecl_device_id* out_devices,
                    ecl_uint* num_devices_ret) ECL_API_SUFFIX__VERSION_1_2 {
    if (!IsDeviceValid(in_device)) return ECL_INVALID_DEVICE;

    if (num_devices_ret) *num_devices_ret = 0;

    return ECL_INVALID_VALUE;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainDevice(ecl_device_id device) ECL_API_SUFFIX__VERSION_1_2 {
    if (!IsDeviceValid(device)) return ECL_INVALID_DEVICE;

    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseDevice(ecl_device_id device) ECL_API_SUFFIX__VERSION_1_2 {
    if (!IsDeviceValid(device)) return ECL_INVALID_DEVICE;

    return ECL_SUCCESS;
}
