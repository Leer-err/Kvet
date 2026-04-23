#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

#include <cstddef>

#include "Semaphore.h"


namespace Graphics::Internal {

struct Texture {
    VkImage image;
    VkImageLayout layout;
    VmaAllocation allocation;

    VkFormat format;
    size_t width;
    size_t height;

    Semaphore is_ready;
};

}  // namespace Graphics::Internal