// Copyright 2018 RnD Center "ELVEES", JSC

#include <mutex>
#include <utility>
#include <vector>

#include <boost/intrusive_ptr.hpp>

#include "elcore-cl.h"

std::pair<ecl_int, bool> CheckIfNodeNeedToWaitEvent(ecl_command_queue_node *node,
                                                    _ecl_event *event) {
    bool need_to_wait_before_submit = false;
    ecl_int command_status;
    if (node->m_event->m_command_type == ECL_COMMAND_NDRANGE_KERNEL) {
        if (event->m_command_type == ECL_COMMAND_NDRANGE_KERNEL &&
            event->m_queue == node->m_queue) {
            if (event->m_command_status > ECL_SUBMITTED) {
                need_to_wait_before_submit = true;
                command_status = ECL_SUBMITTED;
            }
        } else {
            if (event->m_command_status > ECL_COMPLETE) {
                need_to_wait_before_submit = true;
                command_status = ECL_COMPLETE;
            }
        }
    } else {
        if (event->m_command_status > ECL_COMPLETE) {
            need_to_wait_before_submit = true;
            command_status = ECL_COMPLETE;
        }
    }
    return std::make_pair(command_status, need_to_wait_before_submit);
}

void AdvanceCommandQueueNode(ecl_command_queue_node *node) {
    bool need_to_wait_before_submit = false;
    for (std::size_t i = 0; i < node->m_wait_list.size(); ++i) {
        boost::intrusive_ptr<_ecl_event> event = node->m_wait_list[i];
        std::unique_lock<std::mutex> lock(event->m_mutex);
        need_to_wait_before_submit |= CheckIfNodeNeedToWaitEvent(node, event.get()).second;
    }
    ecl_map_flags flags;
    size_t offset, cb;
    ecl_mem mem;
    if (!need_to_wait_before_submit) {
        switch (node->m_event->m_command_type) {
        case ECL_COMMAND_NDRANGE_KERNEL:
            if (node->m_queue->m_device->enqueue_job(&node->m_kernel->m_job_instance) != 0)
                return;  // ECL_INVALID_CONTEXT;  //@TODO: return correct error
            node->m_event->set_command_status(ECL_SUBMITTED);
            {
                std::unique_lock<std::mutex> lock(node->m_queue->m_commands_list_guard);
                node->m_queue->m_submitted_kernels_list.push_back(node);
                node->m_queue->m_last_command_in_queue = node;
            }
            node->m_queue->m_commands_list_cond.notify_all();
            break;
        case ECL_COMMAND_MAP_BUFFER:
            offset = node->m_sync_object->m_offset;
            cb = node->m_sync_object->m_cb;
            flags = node->m_sync_object->m_map_flags;
            mem = node->m_sync_object->m_memobj;
            if (mem->sync_for_host(offset, cb, flags) != 0)
                return;
            node->m_event->set_command_status(ECL_COMPLETE);
            break;
        case ECL_COMMAND_UNMAP_MEM_OBJECT:
            mem = node->m_sync_object->m_memobj;
            if (mem->sync_for_device() != 0)
                return;
            node->m_event->set_command_status(ECL_COMPLETE);
            break;
        default:
            node->m_event->set_command_status(ECL_COMPLETE);
            return;
        }
    }
}

void ECL_CALLBACK WaitNodeCallback(ecl_event, ecl_int, void *user_data) {
    // @TODO: check errors
    ecl_command_queue_node *node = reinterpret_cast<ecl_command_queue_node *>(user_data);
    AdvanceCommandQueueNode(node);

    {
        std::unique_lock<std::mutex> lock(node->m_queue->m_commands_list_guard);
        if (node->m_queue->m_last_command_in_queue == node)
            node->m_queue->m_last_command_in_queue = nullptr;
    }
    node->release();
}

bool FillWaitListCallback(boost::intrusive_ptr<ecl_command_queue_node> node) {
    bool need_to_wait_before_submit = false;
    for (std::size_t i = 0; i < node->m_wait_list.size(); ++i) {
        boost::intrusive_ptr<_ecl_event> event = node->m_wait_list[i];
        std::unique_lock<std::mutex> lock(event->m_mutex);
        std::pair<ecl_int, bool> check_result = CheckIfNodeNeedToWaitEvent(node.get(), event.get());
        need_to_wait_before_submit |= check_result.second;
        if (check_result.second) {
            node->add_ref();
            event->m_callbacks[check_result.first]
            .push_back(std::pair<void(ECL_CALLBACK *)(ecl_event, ecl_int, void *), void *>(
                       WaitNodeCallback, node.get()));
        }
    }
    return need_to_wait_before_submit;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueNDRangeKernel(ecl_command_queue command_queue, ecl_kernel kernel, ecl_uint work_dim,
                        const size_t *global_work_offset, const size_t *global_work_size,
                        const size_t *local_work_size, ecl_uint num_events_in_wait_list,
                        const ecl_event *event_wait_list,
                        ecl_event *event) ECL_API_SUFFIX__VERSION_1_0 {
    if (command_queue == nullptr) return ECL_INVALID_COMMAND_QUEUE;
    if (kernel == nullptr) return ECL_INVALID_KERNEL;
    if (command_queue->m_context != kernel->m_program->m_context) return ECL_INVALID_CONTEXT;

    if (event_wait_list == nullptr && num_events_in_wait_list > 0)
        return ECL_INVALID_EVENT_WAIT_LIST;

    if (event_wait_list != nullptr && num_events_in_wait_list == 0)
        return ECL_INVALID_EVENT_WAIT_LIST;

    if (event_wait_list) {
        for (ecl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (event_wait_list[i] == nullptr) return ECL_INVALID_EVENT_WAIT_LIST;
            if (event_wait_list[i]->m_context != command_queue->m_context)
                return ECL_INVALID_CONTEXT;
        }
    }

    if (work_dim != 1) return ECL_INVALID_WORK_DIMENSION;
    if (global_work_size == nullptr) return ECL_INVALID_GLOBAL_WORK_SIZE;
    if (global_work_size[0] != 1) return ECL_INVALID_GLOBAL_WORK_SIZE;
    if (global_work_offset != nullptr) return ECL_INVALID_GLOBAL_OFFSET;

    {
        std::unique_lock<std::mutex> lock(kernel->m_kernel_guard);
        if (!kernel->m_args.empty()) {
            auto next_it = kernel->m_args.cbegin();
            for (auto it = next_it++; next_it != kernel->m_args.cend(); ++it, ++next_it) {
                if (it->first != (next_it->first - 1)) return ECL_INVALID_KERNEL_ARGS;
            }
        }
    }

    if (local_work_size != nullptr) return ECL_INVALID_WORK_ITEM_SIZE;
    // @TODO: check if we can map memories to virtual space
    // else return ECL_MEM_OBJECT_ALLOCATION_FAILURE

    const boost::intrusive_ptr<_ecl_program> program = kernel->m_program;

    int dev_num = -1;
    for (int dev = 0; dev < program->m_devices.size(); dev++) {
        if (program->m_devices[dev] == command_queue->m_device_id) {
            dev_num = dev;
            break;
        }
    }
    if (dev_num == -1) {
        return ECL_INVALID_CONTEXT;
    }

    boost::intrusive_ptr<ecl_command_queue_node> node = new ecl_command_queue_node();
    node->m_kernel = new _ecl_kernel_instance();
    node->m_kernel->m_job_instance = program->m_job_instance_templates[dev_num];
    elcore50_job_instance &job_instance = node->m_kernel->m_job_instance;

    {
        std::unique_lock<std::mutex> lock(kernel->m_kernel_guard);
        job_instance.argc = kernel->m_args.size();
        for (auto it = kernel->m_args.cbegin(); it != kernel->m_args.cend(); ++it) {
            const ecl_uint index = it->first;
            const ecl_uint type = it->second.first;
            switch (type) {
                case ECL_KERNEL_ARG_TYPE_BASIC: {
                    job_instance.args[index].type = ELCORE50_TYPE_BASIC;
                    node->m_basic_args_copy.push_back(it->second.second);
                    const std::vector<uint8_t> &data = node->m_basic_args_copy.back();
                    job_instance.args[index].basic.p = reinterpret_cast<uintptr_t>(data.data());
                    job_instance.args[index].basic.size = data.size();
                    break;
                }
                case ECL_KERNEL_ARG_TYPE_NC_GLOBAL:
                case ECL_KERNEL_ARG_TYPE_GLOBAL: {
                    if (type == ECL_KERNEL_ARG_TYPE_GLOBAL)
                        job_instance.args[index].type = ELCORE50_TYPE_GLOBAL_MEMORY;
                    else
                        job_instance.args[index].type = ELCORE50_TYPE_NC_GLOBAL_MEMORY;
                    node->m_mem_args_ref.emplace_back(
                        *reinterpret_cast<const ecl_mem *>(it->second.second.data()));
                    ecl_mem mem = node->m_mem_args_ref.back().get();
                    job_instance.args[index].global_memory.mapper_fd = mem->m_mem_info.mapper_fd;
                    break;
                }
                case ECL_KERNEL_ARG_TYPE_LOCAL:
                    job_instance.args[index].type = ELCORE50_TYPE_LOCAL_MEMORY;
                    job_instance.args[index].local_memory.size =
                        *reinterpret_cast<const size_t *>(it->second.second.data());
                    break;
                case ECL_KERNEL_ARG_TYPE_DMA: {
                    job_instance.args[index].type = ELCORE50_TYPE_DMA_MEMORY;
                    node->m_mem_args_ref.emplace_back(
                        *reinterpret_cast<const ecl_mem *>(it->second.second.data()));
                    ecl_mem mem = node->m_mem_args_ref.back().get();
                    job_instance.args[index].dma_memory.mapper_fd = mem->m_mem_info.mapper_fd;
                    break;
                }
            }
        }
    }
    job_instance.entry_point_virtual_address = kernel->m_kernel_addr;
    strncpy(job_instance.name, kernel->m_kernel_name.c_str(), sizeof(job_instance.name));
    job_instance.debug_enable = 0;

    node->m_event =
        new _ecl_event(command_queue->m_context.get(), command_queue, ECL_COMMAND_NDRANGE_KERNEL);
    node->m_queue = command_queue;
    if (num_events_in_wait_list) {
        node->m_wait_list.assign(event_wait_list, event_wait_list + num_events_in_wait_list);
    }
    {
        std::unique_lock<std::mutex> lock(command_queue->m_commands_list_guard);
        if (command_queue->m_last_command_in_queue != nullptr) {
            node->m_wait_list.push_back(command_queue->m_last_command_in_queue->m_event);
        }
    }
    if (!FillWaitListCallback(node)) {
        if (command_queue->m_device->enqueue_job(&node->m_kernel->m_job_instance) != 0) {
            return ECL_INVALID_CONTEXT;  // @TODO: return correct error
        }
        node->m_event->set_command_status(ECL_SUBMITTED);
        {
            std::unique_lock<std::mutex> lock(command_queue->m_commands_list_guard);
            command_queue->m_submitted_kernels_list.push_back(node);
            command_queue->m_last_command_in_queue = node;
        }
        command_queue->m_commands_list_cond.notify_all();
    }
    if (event) {
        node->m_event->add_ref();
        *event = node->m_event.get();
    }
    return ECL_SUCCESS;
}

extern "C" ECL_API_ENTRY void * ECL_API_CALL
eclEnqueueMapBuffer(ecl_command_queue command_queue, ecl_mem memobj, ecl_bool blocking_map,
                    ecl_map_flags map_flags, size_t offset, size_t cb,
                    ecl_uint num_events_in_wait_list, const ecl_event *event_wait_list,
                    ecl_event *event, ecl_int *errcode_ret) ECL_API_SUFFIX__VERSION_1_0 {
    if (command_queue == nullptr) {
        *errcode_ret = ECL_INVALID_COMMAND_QUEUE;
        return NULL;
    }

    if (memobj == nullptr) {
        *errcode_ret = ECL_INVALID_MEM_OBJECT;
        return NULL;
    }

    if (memobj->m_host_ptr == nullptr) {
        *errcode_ret = ECL_INVALID_OPERATION;
        return NULL;
    }

    if ((cb == 0) || (offset + cb > memobj->m_size)) {
        *errcode_ret = ECL_INVALID_ARG_SIZE;
        return NULL;
    }

    if (map_flags & ~(ECL_MAP_READ | ECL_MAP_WRITE)) {
        *errcode_ret =  ECL_INVALID_VALUE;
        return NULL;
    }

    if (event_wait_list == nullptr && num_events_in_wait_list > 0) {
        *errcode_ret = ECL_INVALID_EVENT_WAIT_LIST;
        return NULL;
    }

    if (event_wait_list != nullptr && num_events_in_wait_list == 0) {
        *errcode_ret = ECL_INVALID_EVENT_WAIT_LIST;
        return NULL;
    }

    if (event_wait_list) {
        for (ecl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (event_wait_list[i] == nullptr) {
                *errcode_ret = ECL_INVALID_EVENT_WAIT_LIST;
                return NULL;
            }
            if (event_wait_list[i]->m_context != command_queue->m_context) {
                *errcode_ret = ECL_INVALID_CONTEXT;
                return NULL;
            }
        }
    }

    boost::intrusive_ptr<ecl_command_queue_node> node = new ecl_command_queue_node();
    node->m_event =
        new _ecl_event(command_queue->m_context.get(), command_queue, ECL_COMMAND_MAP_BUFFER);
    node->m_queue = command_queue;
    if (num_events_in_wait_list) {
        node->m_wait_list.assign(event_wait_list, event_wait_list + num_events_in_wait_list);
    }
    {
        std::unique_lock<std::mutex> lock(command_queue->m_commands_list_guard);
        if (command_queue->m_last_command_in_queue != nullptr) {
            node->m_wait_list.push_back(command_queue->m_last_command_in_queue->m_event);
        }
    }
    boost::intrusive_ptr<_ecl_sync_object_instance> sync = new _ecl_sync_object_instance();
    sync->m_memobj = memobj;
    sync->m_map_flags = map_flags;
    sync->m_cb = cb;
    sync->m_offset = offset;

    node->m_sync_object = sync;

    if (!FillWaitListCallback(node)) {
        if (memobj->sync_for_host(offset, cb, map_flags) != 0) {
            *errcode_ret = ECL_DEVICE_NOT_AVAILABLE;
            return NULL;
        }
        node->m_event->set_command_status(ECL_COMPLETE);
    }
    if (event) {
        node->m_event->add_ref();
        *event = node->m_event.get();
    }

    if (blocking_map) {
        ecl_event block_event = node->m_event.get();
        eclWaitForEvents(1, &block_event);
    }

    if (errcode_ret) *errcode_ret = ECL_SUCCESS;
    return memobj->m_host_ptr + offset;
}

extern "C" ECL_API_ENTRY ecl_int ECL_API_CALL
eclEnqueueUnmapMemObject(ecl_command_queue command_queue, ecl_mem memobj, void *mapped_ptr,
                         ecl_uint num_events_in_wait_list, const ecl_event *event_wait_list,
                         ecl_event *event) ECL_API_SUFFIX__VERSION_1_0 {
    if (command_queue == nullptr) return ECL_INVALID_COMMAND_QUEUE;
    if (memobj == nullptr) return ECL_INVALID_MEM_OBJECT;

    if (event_wait_list == nullptr && num_events_in_wait_list > 0)
        return ECL_INVALID_EVENT_WAIT_LIST;

    if (event_wait_list != nullptr && num_events_in_wait_list == 0)
        return ECL_INVALID_EVENT_WAIT_LIST;

    if (event_wait_list) {
        for (ecl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (event_wait_list[i] == nullptr) return ECL_INVALID_EVENT_WAIT_LIST;
            if (event_wait_list[i]->m_context != command_queue->m_context)
                return ECL_INVALID_CONTEXT;
        }
    }

    boost::intrusive_ptr<ecl_command_queue_node> node = new ecl_command_queue_node();
    node->m_event =
        new _ecl_event(command_queue->m_context.get(), command_queue,
                       ECL_COMMAND_UNMAP_MEM_OBJECT);
    node->m_queue = command_queue;
    if (num_events_in_wait_list) {
        node->m_wait_list.assign(event_wait_list, event_wait_list + num_events_in_wait_list);
    }
    {
        std::unique_lock<std::mutex> lock(command_queue->m_commands_list_guard);
        if (command_queue->m_last_command_in_queue != nullptr) {
            node->m_wait_list.push_back(command_queue->m_last_command_in_queue->m_event);
        }
    }
    boost::intrusive_ptr<_ecl_sync_object_instance> sync = new _ecl_sync_object_instance();
    sync->m_memobj = memobj;

    node->m_sync_object = sync;

    if (!FillWaitListCallback(node)) {
        if (memobj->sync_for_device() != 0) {
            return ECL_DEVICE_NOT_AVAILABLE;
        }
        node->m_event->set_command_status(ECL_COMPLETE);
    }
    if (event) {
        node->m_event->add_ref();
        *event = node->m_event.get();
    }

    return ECL_SUCCESS;
}
