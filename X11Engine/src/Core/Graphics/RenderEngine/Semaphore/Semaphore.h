#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "APIData.h"

namespace Graphics {

struct Semaphore {
    static Semaphore create(const APIData& api_data);
    static Semaphore create(VkDevice device);

    void destroy();

    VkSemaphoreSubmitInfo submit() const;

    VkDevice device;
    VkSemaphore semaphore;
};

}  // namespace Graphics