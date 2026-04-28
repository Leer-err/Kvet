#pragma once

#include <vulkan/vulkan.h>

namespace Graphics {

struct Semaphore {
    static Semaphore create();
    static Semaphore create(VkDevice device);

    void destroy();

    VkSemaphoreSubmitInfo submit() const;

    VkSemaphore semaphore;
};

}  // namespace Graphics