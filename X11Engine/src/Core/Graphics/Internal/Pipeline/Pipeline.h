#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics {

struct Pipeline {
    void destroy();

    VkPipeline pipeline;
};

}  // namespace Graphics