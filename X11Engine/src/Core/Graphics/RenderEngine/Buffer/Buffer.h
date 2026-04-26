#pragma once

#include <stddef.h>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

namespace Graphics {

struct Buffer {
    void destroy();

    size_t getSize() const;

    void* map();
    void unmap();

    VkDeviceAddress getDeviceAddress() const;

    VkBuffer buffer;
    VmaAllocation allocation;
    size_t size;
};

}  // namespace Graphics
