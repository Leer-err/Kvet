#include "Semaphore.h"

#include <vulkan/vulkan.h>

#include "APIData.h"

namespace Graphics {

Semaphore Semaphore::create(const APIData& api_data) {
    return create(api_data.device);
}

Semaphore Semaphore::create(VkDevice device) {
    Semaphore semaphore;
    semaphore.device = device;

    VkSemaphoreCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_SEMAPHORE_TYPE_BINARY;

    vkCreateSemaphore(device, &info, nullptr, &semaphore.semaphore);

    return semaphore;
}

void Semaphore::destroy() { vkDestroySemaphore(device, semaphore, nullptr); }

VkSemaphoreSubmitInfo Semaphore::submit() const {
    VkSemaphoreSubmitInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    info.semaphore = semaphore;

    return info;
}

}  // namespace Graphics