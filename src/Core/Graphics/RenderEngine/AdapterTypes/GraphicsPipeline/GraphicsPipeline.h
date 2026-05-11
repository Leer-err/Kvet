#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace Graphics {

struct GraphicsPipeline {
    VkPipeline pipeline;
    VkPipelineLayout layout;
};

}  // namespace Graphics