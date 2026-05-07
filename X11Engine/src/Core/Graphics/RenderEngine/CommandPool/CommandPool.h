#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "CommandBuffer.h"
#include "EngineData.h"

namespace Graphics {

class CommandPool {
   public:
    CommandPool(const EngineData& engine_data, uint32_t queue_index);

    CommandBuffer getCommandBuffer();
    CommandBuffer createCommandBuffer();

    void reset();

   private:
    const EngineData& engine_data;

    VkCommandPool pool;
    std::vector<CommandBuffer> command_buffers;
    uint32_t unused_buffer_index;
};

}  // namespace Graphics