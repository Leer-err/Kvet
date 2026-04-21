#include "Pipeline.h"

#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"


namespace Graphics::Internal {

void Pipeline::destroy() {
    vkDestroyPipeline(Resources::get().getDevice(), pipeline, nullptr);
}

}  // namespace Graphics::Internal