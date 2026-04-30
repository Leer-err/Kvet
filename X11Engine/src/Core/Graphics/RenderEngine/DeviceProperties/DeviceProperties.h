#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string>

namespace Graphics {

struct DeviceProperties {
    static DeviceProperties readProperties(VkPhysicalDevice device);

    std::string device_name;
    size_t max_descriptor_set_size;

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