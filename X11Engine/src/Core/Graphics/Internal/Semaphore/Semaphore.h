#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

struct Semaphore {
    Semaphore();
    ~Semaphore();

    VkSemaphore semaphore;
};

}  // namespace Graphics::Internal