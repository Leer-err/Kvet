#include "InternalTexture.h"

#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"

namespace Graphics::Internal {

void Texture::destroy() {
    Resources::get().getAllocator();

    vmaDestroyImage(Resources::get().getAllocator(), image, allocation);
}

}  // namespace Graphics::Internal