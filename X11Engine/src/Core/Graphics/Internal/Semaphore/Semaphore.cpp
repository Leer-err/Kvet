#include "Semaphore.h"

#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"

namespace Graphics::Internal {

Semaphore Semaphore::create() {
    auto device = Resources::get().getDevice();
    return create(device);
}

Semaphore Semaphore::create(VkDevice device) {
    Semaphore semaphore;

    VkSemaphoreCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_SEMAPHORE_TYPE_BINARY;

    vkCreateSemaphore(device, &info, nullptr, &semaphore.semaphore);

    return semaphore;
}

}  // namespace Graphics::Internal