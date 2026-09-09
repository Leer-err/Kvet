#pragma once

#include <vulkan/vulkan.h>

#include "Handles.h"

namespace Graphics {

struct GraphicsPipeline {
    VkPipeline pipeline;
    VkPipelineLayout layout;
    BufferHandle descriptors;
};

}  // namespace Graphics