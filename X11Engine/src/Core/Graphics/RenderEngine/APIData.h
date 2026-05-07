#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include "DeviceProperties.h"

namespace Graphics {

constexpr auto TEXTURE_BINDING_INDEX = 0;
constexpr auto SAMPLER_BINDING_INDEX = 1;

constexpr auto MAX_TEXTURE_DESCRIPTORS_COUNT = 1000;
constexpr auto MAX_SAMPLE_DESCRIPTORS_COUNT = 1000;

// Low-level vulkan API data
struct APIData {
    VkDevice device;
    DeviceProperties properties;

    VkDescriptorSetLayout descriptor_layout;

    VmaAllocator allocator;
};

}  // namespace Graphics