#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics {

struct GraphicsPipeline {
    void destroy();

    VkPipeline pipeline;
    VkPipelineLayout layout;
};

}  // namespace Graphics