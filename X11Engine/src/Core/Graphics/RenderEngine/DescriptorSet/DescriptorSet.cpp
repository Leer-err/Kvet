#include "DescriptorSet.h"

#include <vulkan/vulkan_core.h>

#include "Buffer.h"
#include "BufferBuilder.h"
#include "DeviceProperties.h"
#include "ExtensionFunctions.h"
#include "Sampler.h"

namespace Graphics {

DescriptorSet::DescriptorSet(const Device& device,
                             const DeviceProperties& device_properties)
    : device(device) {
    auto& descriptor_properties =
        device_properties.descriptor_buffer_properties;

    auto descriptor_layout = device.getDescriptorLayout();

    texture_descriptor_size = descriptor_properties.texture_size;
    sampler_descriptor_size = descriptor_properties.sampler_size;

    size_t set_size = descriptor_layout.layout_size;
    auto alignment = descriptor_properties.alignment;
    set_size = (set_size + alignment - 1) & ~(alignment - 1);

    current_sampler_index = 0;
    current_texture_index = 0;
    descriptors = BufferBuilder(device, set_size)
                      .isDescriptorBuffer()
                      .isCPUWritable(true)
                      .create()
                      .getResult();
}

void DescriptorSet::addImage(const VkImageView& texture) {
    auto descriptors_ptr = device.map(descriptors);

    char* binding_ptr =
        static_cast<char*>(descriptors_ptr) + texture_descriptors_offset;
    char* element_ptr =
        binding_ptr + (current_texture_index * texture_descriptor_size);
    current_texture_index++;

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
}

void DescriptorSet::addSampler(const Sampler& sampler) {
    auto descriptors_ptr = device.map(descriptors);

    char* binding_ptr =
        static_cast<char*>(descriptors_ptr) + sampler_descriptors_offset;
    char* element_ptr =
        binding_ptr + (current_sampler_index * sampler_descriptor_size);
    current_sampler_index++;

    VkDescriptorGetInfoEXT info{};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT;
    info.type = VK_DESCRIPTOR_TYPE_SAMPLER;
    info.data.pSampler = &sampler.sampler;
    device.writeDescriptor(info, sampler_descriptor_size, element_ptr);

    device.unmap(descriptors);
}

VkDeviceAddress DescriptorSet::getDescriptors() const {
    return descriptors.device_address;
}

}  // namespace Graphics