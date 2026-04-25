#include "GraphicsPipeline.h"

#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"

namespace Graphics {

void GraphicsPipeline::destroy() {
    auto device = Resources::get().getDevice();
    vkDestroyPipeline(device, pipeline, nullptr);
}

}  // namespace Graphics