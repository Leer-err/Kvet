#pragma once

#include <vulkan/vulkan.h>

namespace Graphics {

struct GraphicsPipeline {
    void destroy();

    VkPipeline pipeline;
    VkPipelineLayout layout;
};

}  // namespace Graphics