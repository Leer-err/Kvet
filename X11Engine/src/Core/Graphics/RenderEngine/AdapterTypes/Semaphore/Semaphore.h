#pragma once

#include <vulkan/vulkan.h>

namespace Graphics {

struct Semaphore {
    VkSemaphoreSubmitInfo submit() const;

    VkSemaphore semaphore;
};

}  // namespace Graphics