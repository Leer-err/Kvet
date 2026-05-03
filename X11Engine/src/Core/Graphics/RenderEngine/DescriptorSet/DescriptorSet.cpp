#include "DescriptorSet.h"

#include <vulkan/vulkan_core.h>

#include "Buffer.h"
#include "BufferBuilder.h"
#include "ExtensionFunctions.h"
#include "GraphicsResources.h"
#include "Sampler.h"
#include "TextureView.h"

namespace Graphics {

constexpr auto CONSTANT_BUFFER_BINDING_INDEX = 0;
constexpr auto TEXTURE_BINDING_INDEX = 1;
constexpr auto SAMPLER_BINDING_INDEX = 2;

DescriptorSet DescriptorSet::create() {
    DescriptorSet set = {};
    auto descriptor_properties =
        Resources::get().getProperties().descriptor_buffer_properties;
    set.texture_descriptor_size = descriptor_properties.texture_size;
    set.sampler_descriptor_size = descriptor_properties.sampler_size;
    auto device = Resources::get().getDevice();
    auto device_properties = Resources::get().getProperties();

    VkDescriptorSetLayoutBinding bindings[3] = {};
    bindings[CONSTANT_BUFFER_BINDING_INDEX].descriptorType =
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[CONSTANT_BUFFER_BINDING_INDEX].binding =
        CONSTANT_BUFFER_BINDING_INDEX;
    bindings[CONSTANT_BUFFER_BINDING_INDEX].stageFlags =
        VK_SHADER_STAGE_ALL_GRAPHICS;
    bindings[CONSTANT_BUFFER_BINDING_INDEX].descriptorCount = 1000;

    bindings[TEXTURE_BINDING_INDEX].descriptorType =
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    bindings[TEXTURE_BINDING_INDEX].binding = TEXTURE_BINDING_INDEX;
    bindings[TEXTURE_BINDING_INDEX].stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
    bindings[TEXTURE_BINDING_INDEX].descriptorCount = 1000;

    bindings[SAMPLER_BINDING_INDEX].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    bindings[SAMPLER_BINDING_INDEX].binding = SAMPLER_BINDING_INDEX;
    bindings[SAMPLER_BINDING_INDEX].stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
    bindings[SAMPLER_BINDING_INDEX].descriptorCount = 1000;

    VkDescriptorSetLayoutCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT;
    info.bindingCount = 3;
    info.pBindings = bindings;
    vkCreateDescriptorSetLayout(device, &info, nullptr, &set.layout);

    size_t set_size;
    vkGetDescriptorSetLayoutSizeEXT(device, set.layout, &set_size);
    auto alignment = device_properties.descriptor_buffer_properties.alignment;
    set_size = (set_size + alignment - 1) & ~(alignment - 1);

    set.current_sampler_index = 0;
    set.current_texture_index = 0;
    set.descriptors = BufferBuilder(set_size)
                          .isDescriptorBuffer()
                          .isCPUWritable(true)
                          .create()
                          .getResult();

    return set;
}

void DescriptorSet::addImage(const TextureView& texture) {
    auto device = Resources::get().getDevice();

    size_t binding_offset;
    vkGetDescriptorSetLayoutBindingOffsetEXT(
        device, layout, SAMPLER_BINDING_INDEX, &binding_offset);

    auto descriptors_ptr = descriptors.map();

    char* binding_ptr = static_cast<char*>(descriptors_ptr) + binding_offset;
    char* element_ptr =
        binding_ptr + (current_texture_index * sampler_descriptor_size);
    current_texture_index++;

    VkDescriptorImageInfo image_descriptor_info = {};
    image_descriptor_info.imageView = texture.view;
    image_descriptor_info.imageLayout =
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkDescriptorGetInfoEXT info{};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT;
    info.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    info.data.pSampledImage = &image_descriptor_info;
    vkGetDescriptorEXT(device, &info, sampler_descriptor_size, element_ptr);

    descriptors.unmap();
}

void DescriptorSet::addSampler(const Sampler& sampler) {
    auto device = Resources::get().getDevice();

    size_t binding_offset;
    vkGetDescriptorSetLayoutBindingOffsetEXT(
        device, layout, SAMPLER_BINDING_INDEX, &binding_offset);

    auto descriptors_ptr = descriptors.map();

    char* binding_ptr = static_cast<char*>(descriptors_ptr) + binding_offset;
    char* element_ptr =
        binding_ptr + (current_sampler_index * sampler_descriptor_size);
    current_sampler_index++;

    VkDescriptorGetInfoEXT info{};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT;
    info.type = VK_DESCRIPTOR_TYPE_SAMPLER;
    info.data.pSampler = &sampler.sampler;
    vkGetDescriptorEXT(device, &info, sampler_descriptor_size, element_ptr);

    descriptors.unmap();
}

// void DescriptorSet::addBuffer(const Buffer& buffer) {}

}  // namespace Graphics