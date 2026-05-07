#include "DescriptorSet.h"

#include <vulkan/vulkan_core.h>

#include "APIData.h"
#include "Buffer.h"
#include "BufferBuilder.h"
#include "DeviceProperties.h"
#include "ExtensionFunctions.h"
#include "Sampler.h"
#include "TextureView.h"

namespace Graphics {

DescriptorSet DescriptorSet::create(const APIData& api_data) {
    auto& device = api_data.device;
    auto& layout = api_data.descriptor_layout;
    auto& descriptor_properties =
        api_data.properties.descriptor_buffer_properties;

    DescriptorSet set = {};
    set.texture_descriptor_size = descriptor_properties.texture_size;
    set.sampler_descriptor_size = descriptor_properties.sampler_size;

    size_t set_size;
    vkGetDescriptorSetLayoutSizeEXT(device, layout, &set_size);
    auto alignment = descriptor_properties.alignment;
    set_size = (set_size + alignment - 1) & ~(alignment - 1);

    vkGetDescriptorSetLayoutBindingOffsetEXT(
        device, layout, TEXTURE_BINDING_INDEX, &set.texture_descriptors_offset);
    vkGetDescriptorSetLayoutBindingOffsetEXT(
        device, layout, SAMPLER_BINDING_INDEX, &set.sampler_descriptors_offset);

    set.api_data = api_data;
    set.current_sampler_index = 0;
    set.current_texture_index = 0;
    set.descriptors = BufferBuilder(api_data, set_size)
                          .isDescriptorBuffer()
                          .isCPUWritable(true)
                          .create()
                          .getResult();

    return set;
}

void DescriptorSet::addImage(const TextureView& texture) {
    auto descriptors_ptr = descriptors.map();

    char* binding_ptr =
        static_cast<char*>(descriptors_ptr) + texture_descriptors_offset;
    char* element_ptr =
        binding_ptr + (current_texture_index * texture_descriptor_size);
    current_texture_index++;

    VkDescriptorImageInfo image_descriptor_info = {};
    image_descriptor_info.imageView = texture.view;
    image_descriptor_info.imageLayout =
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkDescriptorGetInfoEXT info{};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT;
    info.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    info.data.pSampledImage = &image_descriptor_info;
    vkGetDescriptorEXT(api_data.device, &info, texture_descriptor_size,
                       element_ptr);

    descriptors.unmap();
}

void DescriptorSet::addSampler(const Sampler& sampler) {
    auto descriptors_ptr = descriptors.map();

    char* binding_ptr =
        static_cast<char*>(descriptors_ptr) + sampler_descriptors_offset;
    char* element_ptr =
        binding_ptr + (current_sampler_index * sampler_descriptor_size);
    current_sampler_index++;

    VkDescriptorGetInfoEXT info{};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT;
    info.type = VK_DESCRIPTOR_TYPE_SAMPLER;
    info.data.pSampler = &sampler.sampler;
    vkGetDescriptorEXT(api_data.device, &info, sampler_descriptor_size,
                       element_ptr);

    descriptors.unmap();
}

}  // namespace Graphics