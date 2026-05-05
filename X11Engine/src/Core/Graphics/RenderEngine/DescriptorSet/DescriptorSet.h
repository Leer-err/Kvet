#pragma once

#include <vulkan/vulkan.h>

#include "Buffer.h"
#include "Sampler.h"
#include "TextureView.h"

namespace Graphics {

struct DescriptorSet {
    static DescriptorSet create();

    void addImage(const TextureView& texture);
    void addSampler(const Sampler& texture);

    Buffer descriptors;
    size_t current_texture_index;
    size_t current_sampler_index;

    size_t texture_descriptor_size;
    size_t sampler_descriptor_size;

    size_t texture_descriptors_offset;
    size_t sampler_descriptors_offset;

    VkDescriptorSetLayout layout;
};

}  // namespace Graphics