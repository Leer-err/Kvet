#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

struct Fence {
    Fence(bool is_signaled = false);
    ~Fence();

    VkFence fence;
};

}  // namespace Graphics::Internal