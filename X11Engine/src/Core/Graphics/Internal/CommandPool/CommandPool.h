#pragma once

#include <vulkan/vulkan_core.h>

#include "CommandBuffer.h"

namespace Graphics::Internal {

struct CommandPool {
    CommandBuffer getCommandBuffer();

    VkCommandPool pool;
};

}  // namespace Graphics::Internal