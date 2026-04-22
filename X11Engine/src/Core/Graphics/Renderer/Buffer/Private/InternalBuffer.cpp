#include "InternalBuffer.h"

#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"

namespace Graphics::Internal {

void Buffer::destroy() {
    if (buffer != VK_NULL_HANDLE) {
        auto allocator = Resources::get().getAllocator();

        vmaDestroyBuffer(allocator, buffer, allocation);
    }
}

}  // namespace Graphics::Internal