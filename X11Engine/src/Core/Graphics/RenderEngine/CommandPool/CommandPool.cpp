#include "CommandPool.h"

#include <vulkan/vulkan.h>

#include "GraphicsResources.h"

namespace Graphics {

CommandPool CommandPool::create() {
    auto device = Resources::get().getDevice();
    auto queue = Resources::get().getGraphicsQueueIndex();

    VkCommandPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = queue;

    CommandPool pool = {};
    vkCreateCommandPool(device, &info, nullptr, &pool.pool);
    return pool;
}

void CommandPool::destroy() {
    auto device = Resources::get().getDevice();

    vkDestroyCommandPool(device, pool, nullptr);
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

    auto device = Resources::get().getDevice();
    vkAllocateCommandBuffers(device, &info, &buffer.buffer);

    return buffer;
}

void CommandPool::reset() {
    auto device = Resources::get().getDevice();

    vkResetCommandPool(device, pool, 0);

    unused_buffer_index = 0;
}

}  // namespace Graphics