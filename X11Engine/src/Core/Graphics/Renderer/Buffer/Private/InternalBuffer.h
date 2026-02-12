#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

struct Buffer {
    Buffer() noexcept;
    ~Buffer() noexcept;

    VkBuffer buffer;
    VmaAllocation allocation;
    size_t size;
};

}  // namespace Graphics::Internal