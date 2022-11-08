// Copyright 2018-2022 RnD Center "ELVEES", JSC
#include <cassert>
#include <ctime>
#include <mutex>
#include <sstream>
#include <stdexcept>

#include "elcore-cl.h"

template <typename T>
void _ecl_command_queueT<T>::PollThread() {
    do {
        boost::intrusive_ptr<ecl_command_queue_node<T>> kernel_to_poll;
        {
            std::unique_lock<std::mutex> lock(m_commands_list_guard);
            while (m_submitted_kernels_list.empty() && ref_count() > 1) {
                m_commands_list_cond.wait(lock);
            }
            if (ref_count() == 1) break;
            assert(!m_submitted_kernels_list.empty());
            kernel_to_poll = m_submitted_kernels_list.front();
            m_submitted_kernels_list.pop_front();
        }
        int job_instance_fd = kernel_to_poll->m_kernel->m_job_instance.job_instance_fd;
        int status = T::JobInstancePoll(job_instance_fd, m_device->m_fd);

        if (m_device->close_job(job_instance_fd) != 0)
            throw std::runtime_error("could not close job FD");
        {
            std::unique_lock<std::mutex> lock(m_commands_list_guard);
            if (kernel_to_poll == m_last_command_in_queue) m_last_command_in_queue = nullptr;
        }
        kernel_to_poll->m_event->set_command_status(status != T::JOB_STATUS_SUCCESS
                                                        ? -1
                                                        : ECL_COMPLETE);
    } while (ref_count() > 1);
    release();
}

extern "C" ECL_API_ENTRY ecl_command_queue ECL_API_CALL
eclCreateCommandQueueWithProperties(ecl_context context, ecl_device_id device,
                                    const ecl_queue_properties* properties,
                                    ecl_int* errcode_ret) ECL_API_SUFFIX__VERSION_2_0 {
    if (context == nullptr) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_CONTEXT;
        return nullptr;
    }

    if (device == nullptr) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_DEVICE;
        return nullptr;
    }

    ecl_bool found = ECL_FALSE;

    for (size_t i = 0; i < context->m_device_ids.size(); i++) {
        if (context->m_device_ids[i] == device) found = ECL_TRUE;
    }

    if (found == ECL_FALSE) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_DEVICE;
        return nullptr;
    }

    ecl_uint i = 0;
    ecl_command_queue_properties queue_props = 0;
    int queue_props_set = 0;
    if (properties) {
        while (properties[i]) {
            switch (properties[i]) {
                case ECL_QUEUE_PROPERTIES:
                    queue_props = (ecl_command_queue_properties)properties[i + 1];
                    queue_props_set = 1;
                    i += 2;
                    break;
                case ECL_QUEUE_SIZE:
                    if (errcode_ret) *errcode_ret = ECL_INVALID_QUEUE_PROPERTIES;
                    return nullptr;
                default:
                    if (errcode_ret) *errcode_ret = ECL_INVALID_VALUE;
                    return nullptr;
            }
        }
    }

    if (queue_props_set) {
        if (queue_props & (ECL_QUEUE_ON_DEVICE | ECL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE |
                           ECL_QUEUE_ON_DEVICE_DEFAULT)) {
            if (errcode_ret) *errcode_ret = ECL_INVALID_QUEUE_PROPERTIES;
            return nullptr;
        }

        const ecl_command_queue_properties valid_prop_flags =
            (ECL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | ECL_QUEUE_PROFILING_ENABLE |
             ECL_QUEUE_ON_DEVICE | ECL_QUEUE_ON_DEVICE_DEFAULT);
        if (queue_props & (~valid_prop_flags)) {
            if (errcode_ret) *errcode_ret = ECL_INVALID_VALUE;
            return nullptr;
        }
    }

    boost::intrusive_ptr<_ecl_command_queue> command_queue(
        (context->getPlatform() == 0)
        ?(ecl_command_queue)new _ecl_command_queueT<elcore50>(device, context, queue_props)
        :(ecl_command_queue)new _ecl_command_queueT<risc1>(device, context, queue_props), false);

    if (errcode_ret != nullptr) *errcode_ret = ECL_SUCCESS;

    command_queue->add_ref();
    return command_queue.get();
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainCommandQueue(ecl_command_queue command_queue) ECL_API_SUFFIX__VERSION_1_0 {
    if (command_queue == nullptr) return ECL_INVALID_COMMAND_QUEUE;
    command_queue->add_ref();
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseCommandQueue(ecl_command_queue command_queue) ECL_API_SUFFIX__VERSION_1_0 {
    if (command_queue == nullptr) return ECL_INVALID_COMMAND_QUEUE;
    command_queue->release();
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetCommandQueueInfo(ecl_command_queue command_queue, ecl_command_queue_info param_name,
                       size_t param_value_size, void* param_value,
                       size_t* param_value_size_ret) ECL_API_SUFFIX__VERSION_1_0 {
    if (command_queue == nullptr) return ECL_INVALID_COMMAND_QUEUE;

    switch (param_name) {
        case ECL_QUEUE_CONTEXT:
            ECL_RETURN_GETINFO(ecl_context, command_queue->m_context.get());
        case ECL_QUEUE_DEVICE:
            ECL_RETURN_GETINFO(ecl_device_id, command_queue->m_device_id);
        case ECL_QUEUE_REFERENCE_COUNT:
            ECL_RETURN_GETINFO(ecl_uint, (ecl_uint)command_queue->ref_count());
        case ECL_QUEUE_PROPERTIES:
            ECL_RETURN_GETINFO(ecl_command_queue_properties, command_queue->m_properties);
    }
    return ECL_INVALID_VALUE;
}
