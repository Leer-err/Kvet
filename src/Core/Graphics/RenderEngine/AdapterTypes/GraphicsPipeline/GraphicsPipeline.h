#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "Buffer.h"

namespace Graphics {

struct GraphicsPipeline {
    VkPipeline pipeline;
    VkPipelineLayout layout;
    Buffer descriptors;
};

}  // namespace Graphics