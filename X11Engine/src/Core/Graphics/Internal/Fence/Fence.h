#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

struct Fence {
    Fence();
    ~Fence();

    VkFence fence;
};

}  // namespace Graphics::Internal