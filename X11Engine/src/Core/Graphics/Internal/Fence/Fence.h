#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

struct Fence {
    Fence(bool is_signaled = false);
    ~Fence();

    Fence(Fence&& other);
    Fence& operator=(Fence&& other);

    void reset();

    VkFence fence;
};

}  // namespace Graphics::Internal