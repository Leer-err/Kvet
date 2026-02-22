#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

struct CommandBuffer {
    void begin();
    void end();

    void reset();

    VkCommandBuffer buffer;
};

}  // namespace Graphics::Internal