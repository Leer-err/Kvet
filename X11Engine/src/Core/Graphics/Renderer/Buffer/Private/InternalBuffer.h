#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

struct Buffer {
    Buffer() noexcept;
    ~Buffer() noexcept;

    VkBuffer buffer;
    size_t size;
};

}  // namespace Graphics::Internal