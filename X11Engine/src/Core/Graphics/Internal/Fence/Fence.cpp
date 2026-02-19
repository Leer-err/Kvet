#include "Fence.h"

#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"

namespace Graphics::Internal {

Fence::Fence(bool is_signaled) {
    VkFenceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    info.pNext = nullptr;
    if (is_signaled) info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    auto device = Resources::get().getDevice();
    vkCreateFence(device, &info, nullptr, &fence);
}

Fence::~Fence() {
    auto device = Resources::get().getDevice();
    vkDestroyFence(device, fence, nullptr);
}

}  // namespace Graphics::Internal