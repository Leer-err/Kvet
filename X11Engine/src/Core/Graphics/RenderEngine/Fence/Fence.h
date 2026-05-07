#pragma once

#include <vulkan/vulkan.h>

#include "APIData.h"

namespace Graphics {

struct Fence {
    static Fence create(const APIData& api_data, bool is_signaled);
    static Fence create(VkDevice device, bool is_signaled);

    void reset();
    void wait();

    VkFence fence;
    VkDevice device;
};

}  // namespace Graphics