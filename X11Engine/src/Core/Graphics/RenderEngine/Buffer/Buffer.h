#pragma once

#include <stddef.h>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include "APIData.h"
namespace Graphics {

struct Buffer {
    VkBuffer buffer;
    VmaAllocation allocation;

    VkDeviceAddress device_address;
    size_t size;
};

}  // namespace Graphics
