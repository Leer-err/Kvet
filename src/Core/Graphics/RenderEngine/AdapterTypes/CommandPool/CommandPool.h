#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "CommandBuffer.h"
#include "Device.h"

namespace Graphics {

class CommandPool {
   public:
    CommandPool(Device& device, uint32_t queue_index);

    CommandBuffer getCommandBuffer();
    CommandBuffer createCommandBuffer();

    void reset();

   private:
    Device& device;

    VkCommandPool pool;
    std::vector<CommandBuffer> command_buffers;
    uint32_t unused_buffer_index;
};

}  // namespace Graphics