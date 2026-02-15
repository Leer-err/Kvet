#include "InternalTexture.h"

#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"

namespace Graphics::Internal {

Texture::Texture() noexcept : image(VK_NULL_HANDLE) {}

Texture::~Texture() noexcept {
    if (image != VK_NULL_HANDLE) {
        auto allocator = Resources::get().getAllocator();

        vmaDestroyImage(allocator, image, allocation);
    }
}

}  // namespace Graphics::Internal