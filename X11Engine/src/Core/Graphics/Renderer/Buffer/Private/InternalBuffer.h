#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

struct Buffer {
    void destroy();

    VkBuffer buffer;
    VmaAllocation allocation;
    size_t size;
};

}  // namespace Graphics::Internal