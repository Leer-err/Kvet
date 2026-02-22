#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

struct Fence {
    static Fence create(bool is_signaled);
    static Fence create(VkDevice device, bool is_signaled);

    void reset();

    VkFence fence;
};

}  // namespace Graphics::Internal