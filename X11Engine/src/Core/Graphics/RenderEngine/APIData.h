#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include "DeviceProperties.h"

namespace Graphics {

// Low-level vulkan API data
struct APIData {
    VkDevice device;
    DeviceProperties properties;

    VkDescriptorSetLayout descriptor_layout;

    VmaAllocator allocator;
};

}  // namespace Graphics