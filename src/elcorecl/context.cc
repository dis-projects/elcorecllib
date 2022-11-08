// Copyright 2018 RnD Center "ELVEES", JSC

#include <memory>
#include <vector>

#include <boost/intrusive_ptr.hpp>

#include "elcore-cl.h"

extern "C" ECL_API_ENTRY ecl_context ECL_API_CALL
eclCreateContext(const ecl_context_properties *properties, ecl_uint num_devices,
                 const ecl_device_id *devices,
                 void(ECL_CALLBACK *pfn_notify)(const char *, const void *, size_t, void *),
                 void *user_data, ecl_int *errcode_ret) ECL_API_SUFFIX__VERSION_1_0 {
    if (num_devices == 0 || devices == nullptr || properties != nullptr) {
        if (errcode_ret != nullptr) {
            *errcode_ret = ECL_INVALID_ARG_VALUE;
        }
        return nullptr;
    }

    boost::intrusive_ptr<_ecl_context> context(
        (devices[0]->getPlatform()==0)
        ?(ecl_context)new _ecl_contextT<elcore50>(
        std::vector<_ecl_device_id *>(devices, devices + num_devices), pfn_notify, user_data)
        :(ecl_context)new _ecl_contextT<risc1>(
        std::vector<_ecl_device_id *>(devices, devices + num_devices), pfn_notify, user_data));
    context->add_ref();

    if (errcode_ret) *errcode_ret = ECL_SUCCESS;

    return context.get();
}

extern "C" ECL_API_ENTRY ecl_context ECL_API_CALL
eclCreateContextFromType(const ecl_context_properties *properties, ecl_device_type device_type,
                         void(ECL_CALLBACK *pfn_notify)(const char *, const void *, size_t, void *),
                         void *user_data, ecl_int *errcode_ret) ECL_API_SUFFIX__VERSION_1_0 {
    ecl_int errcode = ECL_SUCCESS;
    do {
        ecl_platform_id platform_id;
        errcode = eclGetPlatformIDs(1, &platform_id, nullptr);
        if (errcode != ECL_SUCCESS) break;

        ecl_uint num_devices;
        errcode = eclGetDeviceIDs(platform_id, device_type, 0, nullptr, &num_devices);
        if (errcode != ECL_SUCCESS) break;

        std::unique_ptr<ecl_device_id[]> device_ids(new ecl_device_id[num_devices]);
        errcode = eclGetDeviceIDs(platform_id, device_type, num_devices, device_ids.get(), nullptr);
        if (errcode != ECL_SUCCESS) break;

        return eclCreateContext(properties, num_devices, device_ids.get(), pfn_notify, user_data,
                                errcode_ret);
    } while (false);

    if (errcode_ret != nullptr) *errcode_ret = errcode;
    return nullptr;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainContext(ecl_context context) ECL_API_SUFFIX__VERSION_1_0 {
    if (context == nullptr) return ECL_INVALID_CONTEXT;
    context->add_ref();
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseContext(ecl_context context) ECL_API_SUFFIX__VERSION_1_0 {
    if (context == nullptr) return ECL_INVALID_CONTEXT;
    context->release();
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetContextInfo(ecl_context context, ecl_context_info param_name, size_t param_value_size,
                  void *param_value, size_t *param_value_size_ret) ECL_API_SUFFIX__VERSION_1_0 {
    size_t value_size;

    if (context == nullptr) return ECL_INVALID_CONTEXT;

    switch (param_name) {
        case ECL_CONTEXT_REFERENCE_COUNT:
            ECL_RETURN_GETINFO(ecl_uint, context->ref_count());
        case ECL_CONTEXT_DEVICES:
            value_size = context->m_device_ids.size() * sizeof(ecl_device_id);
            ECL_RETURN_GETINFO_SIZE(value_size, context->m_device_ids.data());
        case ECL_CONTEXT_NUM_DEVICES:
            ECL_RETURN_GETINFO(ecl_uint, context->m_device_ids.size());
        case ECL_CONTEXT_PROPERTIES:
            *param_value_size_ret = 0;
            return ECL_SUCCESS;
    }

    return ECL_INVALID_VALUE;
}
