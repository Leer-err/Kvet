#include "DescriptorSet.h"

#include <vulkan/vulkan_core.h>

#include "Buffer.h"
#include "BufferBuilder.h"
#include "DeviceProperties.h"
#include "ExtensionFunctions.h"
#include "Sampler.h"

namespace Graphics {

DescriptorSet::DescriptorSet(Device& device,
                             const DeviceProperties& device_properties)
    : device(device) {
    auto& descriptor_properties =
        device_properties.descriptor_buffer_properties;

    auto descriptor_layout = device.getDescriptorLayout();

    texture_descriptor_size = descriptor_properties.texture_size;
    sampler_descriptor_size = descriptor_properties.sampler_size;
    texture_descriptors_offset = descriptor_layout.texture_descriptors_offset;
    sampler_descriptors_offset = descriptor_layout.sampler_descriptors_offset;

    size_t set_size = descriptor_layout.layout_size;
    auto alignment = descriptor_properties.alignment;
    set_size = (set_size + alignment - 1) & ~(alignment - 1);

    current_sampler_index = 0;
    current_texture_index = 0;

    VkBufferCreateInfo buffer_info = {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = set_size;
    buffer_info.usage |= VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT |
                         VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
    alloc_info.flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;

    descriptors = device.createBuffer(buffer_info, alloc_info).getResult();
}

TextureHandle DescriptorSet::addImage(const VkImageView& texture) {
    auto descriptors_ptr = device.map(descriptors);

    char* binding_ptr =
        static_cast<char*>(descriptors_ptr) + texture_descriptors_offset;
    char* element_ptr =
        binding_ptr + (current_texture_index * texture_descriptor_size);

    VkDescriptorImageInfo image_descriptor_info = {};
    image_descriptor_info.imageView = texture;
    image_descriptor_info.imageLayout =
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkDescriptorGetInfoEXT info{};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT;
    info.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    info.data.pSampledImage = &image_descriptor_info;
    device.writeDescriptor(info, texture_descriptor_size, element_ptr);

    device.unmap(descriptors);

    auto index = current_texture_index;
    current_texture_index++;

    return index;
}

size_t DescriptorSet::addSampler(const VkSampler& sampler) {
    auto descriptors_ptr = device.map(descriptors);

    char* binding_ptr =
        static_cast<char*>(descriptors_ptr) + sampler_descriptors_offset;
    char* element_ptr =
        binding_ptr + (current_sampler_index * sampler_descriptor_size);

    VkDescriptorGetInfoEXT info{};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT;
    info.type = VK_DESCRIPTOR_TYPE_SAMPLER;
    info.data.pSampler = &sampler;
    device.writeDescriptor(info, sampler_descriptor_size, element_ptr);

    device.unmap(descriptors);

    auto index = current_sampler_index;
    current_sampler_index++;

    return index;
}

VkDeviceAddress DescriptorSet::getDescriptors() const {
    return descriptors.device_address;
}

}  // namespace Graphics