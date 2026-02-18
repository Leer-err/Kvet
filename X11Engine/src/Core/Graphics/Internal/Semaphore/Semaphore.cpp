#include "Semaphore.h"

#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"

namespace Graphics::Internal {

Semaphore::Semaphore() {
    VkSemaphoreCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_SEMAPHORE_TYPE_BINARY;

    auto device = Resources::get().getDevice();

    vkCreateSemaphore(device, &info, nullptr, &semaphore);
}

Semaphore::~Semaphore() {
    auto device = Resources::get().getDevice();

    vkDestroySemaphore(device, semaphore, nullptr);
}

}  // namespace Graphics::Internal