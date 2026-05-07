#include "CommandPool.h"

#include <vulkan/vulkan.h>

namespace Graphics {

CommandPool::CommandPool(const APIData& api_data, uint32_t queue_index) {
    VkCommandPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = queue_index;

    CommandPool pool = {};
    pool.api_data = api_data;
    vkCreateCommandPool(api_data.device, &info, nullptr, &pool.pool);
    return pool;
}

CommandPool::~CommandPool(const APIData& api_data, uint32_t queue_index) {
    VkCommandPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = queue_index;

    CommandPool pool = {};
    pool.api_data = api_data;
    vkCreateCommandPool(api_data.device, &info, nullptr, &pool.pool);
    return pool;
}

void CommandPool::destroy() {
    vkDestroyCommandPool(api_data.device, pool, nullptr);
}

CommandBuffer CommandPool::getCommandBuffer() {
    if (unused_buffer_index == command_buffers.size()) {
        auto new_buffer = createCommandBuffer();

        command_buffers.push_back(new_buffer);
        unused_buffer_index++;

        return new_buffer;
    }

    auto buffer = command_buffers[unused_buffer_index];
    unused_buffer_index++;
    return buffer;
}

CommandBuffer CommandPool::createCommandBuffer() {
    VkCommandBufferAllocateInfo info = {};
    info.commandPool = pool;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandBufferCount = 1;

    CommandBuffer buffer = {};
    vkAllocateCommandBuffers(api_data.device, &info, &buffer.buffer);

    return buffer;
}

void CommandPool::reset() {
    vkResetCommandPool(api_data.device, pool, 0);

    unused_buffer_index = 0;
}

}  // namespace Graphics