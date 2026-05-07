#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "APIData.h"
#include "CommandBuffer.h"

namespace Graphics {

class CommandPool {
   public:
    CommandPool(const APIData& api_data, uint32_t queue_index);
    ~CommandPool();

    CommandBuffer getCommandBuffer();
    CommandBuffer createCommandBuffer();

    void reset();

   private:
    VkCommandPool pool;
    std::vector<CommandBuffer> command_buffers;
    uint32_t unused_buffer_index;
};

}  // namespace Graphics