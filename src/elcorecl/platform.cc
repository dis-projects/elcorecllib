// Copyright 2018-2022 RnD Center "ELVEES", JSC

#include <string>
#include <cstring>

#include "elcore-cl.h"

struct EclPlatformId {
    unsigned long id;
    const char* name;
    const char* version;
    const char* platform_name;
};

struct EclPlatformId Elcore50PlatformId {
    .id = 0,
    .name = "elcore50",
    .version = "OpenCL 2.0 elcore50, no online compiler support",
    .platform_name = "OpenCL Elcore50 library",
};

struct EclPlatformId RISC1PlatformId {
    .id = 1,
    .name = "risc1",
    .version = "OpenCL 2.0 risc1, no online compiler support",
    .platform_name = "OpenCL RISC1 library",
};

_ecl_platform_id _platforms[2] = {{0, &Elcore50PlatformId}, {1, &RISC1PlatformId}};

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetPlatformIDs(ecl_uint num_entries, ecl_platform_id* platforms,
                  ecl_uint* num_platforms) ECL_API_SUFFIX__VERSION_1_0 {
    const unsigned num = 2;
    unsigned i;

    if (platforms != nullptr) {
        if (num_entries < 1) return ECL_INVALID_VALUE;

        for (i = 0; i < num && i < num_entries; ++i) platforms[i] = &_platforms[i];
    }

    if (num_platforms != nullptr) *num_platforms = num;

    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetPlatformInfo(ecl_platform_id platform, ecl_platform_info param_name, size_t param_value_size,
                   void* param_value, size_t* param_value_size_ret) ECL_API_SUFFIX__VERSION_1_0 {
    std::string ret;
    ecl_platform_id tmp_platform;

    if (platform == nullptr) return ECL_INVALID_PLATFORM;

    eclGetPlatformIDs(1, &tmp_platform, nullptr);
    if (platform != tmp_platform) return ECL_INVALID_PLATFORM;

    switch (param_name) {
        case ECL_PLATFORM_PROFILE:
            ret = "EMBEDDED_PROFILE";
            break;

        case ECL_PLATFORM_VERSION:
            ret = platform->eid->version; //"OpenCL 2.0 elcore50, no online compiler support";
            break;

        case ECL_PLATFORM_NAME:
            ret = platform->eid->platform_name; //"OpenCL Elcore50 library";
            break;

        case ECL_PLATFORM_VENDOR:
            ret = "RnD Center \"ELVEES\", JSC";
            break;

        case ECL_PLATFORM_EXTENSIONS:
            ret = "ecl_khr_fp64";
            break;

        default:
            return ECL_INVALID_VALUE;
    }

    if (param_value != nullptr) {
        if (param_value_size < ret.length() + 1) return ECL_INVALID_VALUE;

        strcpy(reinterpret_cast<char*>(param_value), ret.c_str());
    }

    if (param_value_size_ret != nullptr) *param_value_size_ret = ret.length() + 1;

    return ECL_SUCCESS;
}
