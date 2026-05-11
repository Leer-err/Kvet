#include "DeviceProperties.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <array>

namespace Graphics {

DeviceProperties DeviceProperties::readProperties(VkPhysicalDevice device) {
    DeviceProperties properties;

    readDeviceProperties(device, properties);
    readMemoryProperties(device, properties);
    readFormatProperties(device, properties);

    return properties;
}

void DeviceProperties::readDeviceProperties(VkPhysicalDevice device,
                                            DeviceProperties& properties) {
    VkPhysicalDeviceProperties2 device_properties = {};
    device_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;

    VkPhysicalDeviceDescriptorBufferPropertiesEXT buffer_properties = {};
    buffer_properties.sType =
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT;
    device_properties.pNext = &buffer_properties;

    vkGetPhysicalDeviceProperties2(device, &device_properties);

    properties.device_name = device_properties.properties.deviceName;

    auto& descriptor_buffer_properties =
        properties.descriptor_buffer_properties;

    descriptor_buffer_properties.alignment =
        buffer_properties.descriptorBufferOffsetAlignment;
    descriptor_buffer_properties.sampler_size =
        buffer_properties.samplerDescriptorSize;
    descriptor_buffer_properties.texture_size =
        buffer_properties.sampledImageDescriptorSize;
}

void DeviceProperties::readMemoryProperties(VkPhysicalDevice device,
                                            DeviceProperties& properties) {
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(device, &memory_properties);

    size_t memory_size = 0;
    for (uint32_t i = 0; i < memory_properties.memoryHeapCount; i++) {
        VkDeviceSize heap_size = memory_properties.memoryHeaps[i].size;
        VkMemoryHeapFlags flags = memory_properties.memoryHeaps[i].flags;

        if (flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) memory_size += heap_size;
    }

    properties.memory_size = memory_size;
}

void DeviceProperties::readFormatProperties(VkPhysicalDevice device,
                                            DeviceProperties& properties) {
    std::array<VkFormat, 2> possible_depth_formats{VK_FORMAT_D32_SFLOAT_S8_UINT,
                                                   VK_FORMAT_D24_UNORM_S8_UINT};
    for (const auto& format : possible_depth_formats) {
        VkFormatProperties2 formatProperties{
            .sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2};
        vkGetPhysicalDeviceFormatProperties2(device, format, &formatProperties);

        if (formatProperties.formatProperties.optimalTilingFeatures &
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            properties.depth_format = format;
            break;
        }
    }
}

}  // namespace Graphics