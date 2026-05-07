#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <cstddef>

#include "Buffer.h"
#include "Device.h"
#include "Sampler.h"

namespace Graphics {

class DescriptorSet {
   public:
    DescriptorSet(const Device& device,
                  const DeviceProperties& device_properties);

    void addImage(const VkImageView& texture);
    void addSampler(const Sampler& texture);

    VkDeviceAddress getDescriptors() const;

   private:
    const Device& device;

    Buffer descriptors;
    size_t current_texture_index;
    size_t current_sampler_index;

    size_t texture_descriptor_size;
    size_t sampler_descriptor_size;

    size_t texture_descriptors_offset;
    size_t sampler_descriptors_offset;
};

}  // namespace Graphics