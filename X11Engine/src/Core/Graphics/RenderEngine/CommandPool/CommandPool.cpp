#include "CommandPool.h"

#include <vulkan/vulkan.h>

#include "GraphicsResources.h"

namespace Graphics {

CommandBuffer CommandPool::getCommandBuffer() {
    VkCommandBufferAllocateInfo info = {};
    info.commandPool = pool;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandBufferCount = 1;

    CommandBuffer buffer = {};

    auto device = Resources::get().getDevice();
    vkAllocateCommandBuffers(device, &info, &buffer.buffer);

    return buffer;
}

}  // namespace Graphics