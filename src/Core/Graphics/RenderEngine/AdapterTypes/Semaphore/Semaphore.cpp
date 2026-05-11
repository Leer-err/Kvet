#include "Semaphore.h"

#include <vulkan/vulkan.h>

namespace Graphics {

VkSemaphoreSubmitInfo Semaphore::submit() const {
    VkSemaphoreSubmitInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    info.semaphore = semaphore;

    return info;
}

}  // namespace Graphics