#include "Fence.h"

#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"

namespace Graphics::Internal {

Fence Fence::create(bool is_signaled) {
    auto device = Resources::get().getDevice();
    return create(device, is_signaled);
}

Fence Fence::create(VkDevice device, bool is_signaled) {
    Fence fence;

    VkFenceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    info.pNext = nullptr;
    if (is_signaled) info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    vkCreateFence(device, &info, nullptr, &fence.fence);

    return fence;
}

void Fence::reset() {
    auto device = Resources::get().getDevice();
    vkResetFences(device, 1, &fence);
}

}  // namespace Graphics::Internal