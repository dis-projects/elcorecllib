// Copyright 2018 RnD Center "ELVEES", JSC

#include <mutex>
#include <utility>

#include "elcore-cl.h"

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclWaitForEvents(ecl_uint num_events, const ecl_event *event_list) ECL_API_SUFFIX__VERSION_1_0 {
    if (num_events == 0 || event_list == nullptr) return ECL_INVALID_VALUE;

    for (ecl_uint i = 0; i < num_events; ++i) {
        if (event_list[i] == nullptr) return ECL_INVALID_EVENT;
        if (event_list[i]->m_context != event_list[0]->m_context) return ECL_INVALID_CONTEXT;
    }

    ecl_int ret = ECL_SUCCESS;
    for (ecl_uint i = 0; i < num_events; ++i) {
        std::unique_lock<std::mutex> lock(event_list[i]->m_mutex);
        while (event_list[i]->m_command_status > ECL_COMPLETE) {
            event_list[i]->m_status_cond.wait(lock);
        }
        if (event_list[i]->m_command_status < 0)
            ret = ECL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST;
    }
    return ret;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclGetEventInfo(ecl_event event, ecl_event_info param_name, size_t param_value_size,
                void *param_value, size_t *param_value_size_ret) ECL_API_SUFFIX__VERSION_1_0 {
    if (event == nullptr) return ECL_INVALID_EVENT;

    switch (param_name) {
        case ECL_EVENT_COMMAND_EXECUTION_STATUS:
            ECL_RETURN_GETINFO(ecl_int, event->get_command_status());
        case ECL_EVENT_COMMAND_QUEUE:
            ECL_RETURN_GETINFO(ecl_command_queue, event->m_queue.get());
        case ECL_EVENT_COMMAND_TYPE:
            ECL_RETURN_GETINFO(ecl_command_type, event->m_command_type);
        case ECL_EVENT_REFERENCE_COUNT:
            ECL_RETURN_GETINFO(ecl_uint, event->ref_count());
        case ECL_EVENT_CONTEXT:
            ECL_RETURN_GETINFO(ecl_context, event->m_context.get());
    }
    return ECL_INVALID_VALUE;
}

#ifdef ECL_VERSION_1_1

extern "C" ECL_API_ENTRY ecl_event ECL_API_CALL
eclCreateUserEvent(ecl_context context, ecl_int *errcode_ret) ECL_API_SUFFIX__VERSION_1_1 {
    if (context == nullptr) {
        if (errcode_ret) *errcode_ret = ECL_INVALID_CONTEXT;
        return nullptr;
    }
    if (errcode_ret) *errcode_ret = ECL_SUCCESS;
    boost::intrusive_ptr<_ecl_event> event = new _ecl_event(context, nullptr, ECL_COMMAND_USER);
    event->add_ref();
    return event.get();
}

#endif

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclRetainEvent(ecl_event event) ECL_API_SUFFIX__VERSION_1_0 {
    if (event == nullptr) return ECL_INVALID_EVENT;
    event->add_ref();
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclReleaseEvent(ecl_event event) ECL_API_SUFFIX__VERSION_1_0 {
    if (event == nullptr) return ECL_INVALID_EVENT;
    event->release();
    return ECL_SUCCESS;
}

#ifdef ECL_VERSION_1_1

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetUserEventStatus(ecl_event event, ecl_int execution_status) ECL_API_SUFFIX__VERSION_1_1 {
    if (event == nullptr) return ECL_INVALID_EVENT;
    if (execution_status > ECL_COMPLETE) return ECL_INVALID_VALUE;
    if (event->m_command_type != ECL_COMMAND_USER) return ECL_INVALID_EVENT;
    if (event->get_command_status() <= ECL_COMPLETE) return ECL_INVALID_OPERATION;

    event->set_command_status(execution_status);
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclSetEventCallback(ecl_event event, ecl_int command_exec_callback_type,
                    void(ECL_CALLBACK *pfn_notify)(ecl_event, ecl_int, void *),
                    void *user_data) ECL_API_SUFFIX__VERSION_1_1 {
    if (event == nullptr) return ECL_INVALID_EVENT;
    if (pfn_notify == nullptr) return ECL_INVALID_VALUE;

    if (command_exec_callback_type != ECL_QUEUED && command_exec_callback_type != ECL_SUBMITTED &&
        command_exec_callback_type != ECL_RUNNING && command_exec_callback_type != ECL_COMPLETE)
        return ECL_INVALID_VALUE;

    ecl_int status;
    {
        std::unique_lock<std::mutex> lock(event->m_mutex);
        status = event->m_command_status;
        event->m_callbacks[command_exec_callback_type]
            .push_back(std::pair<void(ECL_CALLBACK *)(ecl_event, ecl_int, void *), void *>(
                 pfn_notify, user_data));
    }
    if (status <= command_exec_callback_type) {
        pfn_notify(event, status, user_data);
    }

    return ECL_SUCCESS;
}

#endif
