#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "CommandBuffer.h"

namespace Graphics {

struct CommandPool {
    static CommandPool create();
    void destroy();

    CommandBuffer getCommandBuffer();
    CommandBuffer createCommandBuffer();

    void reset();

    VkCommandPool pool;
    std::vector<CommandBuffer> command_buffers;
    uint32_t unused_buffer_index;
};

}  // namespace Graphics