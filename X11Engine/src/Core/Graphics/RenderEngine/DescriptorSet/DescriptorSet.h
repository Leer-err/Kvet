#pragma once

#include <vulkan/vulkan.h>

#include <cstddef>

#include "APIData.h"
#include "Buffer.h"
#include "Sampler.h"
#include "TextureView.h"

namespace Graphics {

struct DescriptorSet {
    static DescriptorSet create(const APIData& api_data);

    void addImage(const TextureView& texture);
    void addSampler(const Sampler& texture);

    Buffer descriptors;
    size_t current_texture_index;
    size_t current_sampler_index;

    size_t texture_descriptor_size;
    size_t sampler_descriptor_size;

    size_t texture_descriptors_offset;
    size_t sampler_descriptors_offset;
};

}  // namespace Graphics