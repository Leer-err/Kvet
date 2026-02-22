#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

struct Semaphore {
    static Semaphore create();
    static Semaphore create(VkDevice device);

    VkSemaphore semaphore;
};

}  // namespace Graphics::Internal