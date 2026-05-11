#include "CommandPool.h"

#include <vulkan/vulkan.h>

#include "Device.h"

namespace Graphics {

CommandPool::CommandPool(Device& device, uint32_t queue_index)
    : device(device), unused_buffer_index(0) {
    pool = device.createCommandPool(queue_index);
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
    buffer.buffer = device.createCommandBuffer(pool);

    return buffer;
}

void CommandPool::reset() {
    device.resetCommandPool(pool);

    unused_buffer_index = 0;
}

}  // namespace Graphics