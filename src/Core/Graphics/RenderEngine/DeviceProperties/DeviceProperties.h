#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <cstddef>
#include <string>

namespace Graphics {

struct DescriptorBufferProperties {
    size_t alignment;

    size_t sampler_size;
    size_t texture_size;
};

struct DeviceProperties {
    static DeviceProperties readProperties(VkPhysicalDevice device);

    std::string device_name;
    DescriptorBufferProperties descriptor_buffer_properties;

    size_t memory_size;

    VkFormat depth_format;

   private:
    static void readDeviceProperties(VkPhysicalDevice device,
                                     DeviceProperties& properties);
    static void readMemoryProperties(VkPhysicalDevice device,
                                     DeviceProperties& properties);
    static void readFormatProperties(VkPhysicalDevice device,
                                     DeviceProperties& properties);
};

}  // namespace Graphics