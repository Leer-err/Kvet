#pragma once

#include <vulkan/vulkan.h>

#include "Handles.h"

namespace Graphics {

// struct GraphicsPipeline {
//     VkPipeline pipeline;
//     VkPipelineLayout layout;
//     BufferHandle descriptors;
// };

class GraphicsPipeline {
   public:
   private:
    VkPipeline pipeline;
    VkPipelineLayout layout;
};

}  // namespace Graphics