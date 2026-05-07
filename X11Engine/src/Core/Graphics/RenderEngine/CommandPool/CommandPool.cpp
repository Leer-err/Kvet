#include "CommandPool.h"

#include <vulkan/vulkan.h>

namespace Graphics {

CommandPool::CommandPool(const EngineData& engine_data, uint32_t queue_index)
    : engine_data(engine_data) {
    engine_data.device.createCommandPool(queue_index);
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
    buffer.buffer = engine_data.device.createCommandBuffer(pool);

    return buffer;
}

void CommandPool::reset() {
    engine_data.device.resetCommandPool(pool);

    unused_buffer_index = 0;
}

}  // namespace Graphics