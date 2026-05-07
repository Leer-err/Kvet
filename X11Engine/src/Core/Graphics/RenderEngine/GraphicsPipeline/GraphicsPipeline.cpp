#include "GraphicsPipeline.h"

#include <vulkan/vulkan.h>

namespace Graphics {

void GraphicsPipeline::destroy() {
    vkDestroyPipeline(device, pipeline, nullptr);
}

}  // namespace Graphics