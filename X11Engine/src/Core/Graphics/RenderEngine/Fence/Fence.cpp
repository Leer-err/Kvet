#include "Fence.h"

#include <vulkan/vulkan.h>

#include "APIData.h"

namespace Graphics {

Fence Fence::create(const APIData& api_data, bool is_signaled) {
    return create(api_data.device, is_signaled);
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

void Fence::reset() { vkResetFences(device, 1, &fence); }

void Fence::wait() {
    auto result = vkWaitForFences(device, 1, &fence, true, UINT64_MAX);
}

}  // namespace Graphics