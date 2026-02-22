#include "CommandBuffer.h"

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

void CommandBuffer::begin() {
    VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(buffer, &info);
}

void CommandBuffer::end() { vkEndCommandBuffer(buffer); }

void CommandBuffer::reset() { vkResetCommandBuffer(buffer, 0); }

}  // namespace Graphics::Internal