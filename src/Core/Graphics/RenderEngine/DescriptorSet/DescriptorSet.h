#pragma once

#include <vulkan/vulkan.h>

#include <cstddef>
#include <optional>
#include <vector>

#include "Buffer.h"
#include "Descriptors.h"
#include "Device.h"

namespace Graphics {

class DescriptorSet {
   public:
    DescriptorSet(Device& device, const DeviceProperties& device_properties);

    std::optional<TextureDescriptor> addTexture(VkImageView texture_view);
    std::optional<SamplerDescriptor> addSampler(VkSampler sampler);

    void removeTexture(TextureDescriptor texture);
    void removeSampler(SamplerDescriptor sampler);

    VkDeviceAddress getDescriptors() const;

   private:
    static Buffer createDescriptorBuffer(Device& device, size_t set_size,
                                         size_t alignment);

    uint8_t* getTextureDescriptorsData() const;
    uint8_t* getSamplerDescriptorsData() const;

    Device& device;

    Buffer descriptors;

    IndexAllocator<TextureDescriptor> texture_allocator;
    IndexAllocator<SamplerDescriptor> sampler_allocator;

    size_t texture_descriptor_size;
    size_t sampler_descriptor_size;
    size_t texture_descriptors_offset;
    size_t sampler_descriptors_offset;
};

}  // namespace Graphics