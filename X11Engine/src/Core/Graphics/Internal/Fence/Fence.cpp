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
    if (fence == VK_NULL_HANDLE) return;

    auto device = Resources::get().getDevice();
    vkDestroyFence(device, fence, nullptr);
}

Fence::Fence(Fence&& other) : fence(other.fence) {
    other.fence = VK_NULL_HANDLE;
}

Fence& Fence::operator=(Fence&& other) {
    this->fence = other.fence;
    other.fence = VK_NULL_HANDLE;

    return *this;
}

void Fence::reset() {
    auto device = Resources::get().getDevice();
    vkResetFences(device, 1, &fence);
}

}  // namespace Graphics::Internal