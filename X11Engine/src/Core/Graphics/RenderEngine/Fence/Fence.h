#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics {

struct Fence {
    static Fence create(bool is_signaled);
    static Fence create(VkDevice device, bool is_signaled);

    void reset();
    void wait();

    VkFence fence;
};

}  // namespace Graphics