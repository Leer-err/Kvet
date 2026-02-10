#include "InternalBuffer.h"

#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"

namespace Graphics::Internal {

Buffer::Buffer() noexcept : buffer(VK_NULL_HANDLE) {}

Buffer::~Buffer() noexcept {
    if (buffer != VK_NULL_HANDLE) {
        auto device = Resources::get().getDevice();

        vkDestroyBuffer(device, buffer, nullptr);
    }
}

}  // namespace Graphics::Internal