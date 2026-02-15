#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

#include <cstddef>

namespace Graphics::Internal {

struct Texture {
    Texture() noexcept;
    ~Texture() noexcept;

    VkImage image;
    VmaAllocation allocation;

    VkFormat format;
    size_t width;
    size_t height;
};

}  // namespace Graphics::Internal