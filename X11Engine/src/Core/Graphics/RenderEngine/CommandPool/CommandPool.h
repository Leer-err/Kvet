#pragma once

#include <vulkan/vulkan_core.h>

#include "CommandBuffer.h"

namespace Graphics {

struct CommandPool {
    CommandBuffer getCommandBuffer();

    VkCommandPool pool;
};

}  // namespace Graphics