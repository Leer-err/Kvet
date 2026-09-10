#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "Descriptors.h"
#include "Device.h"

namespace Graphics {

class Sampler {
   public:
    static SamplerDescriptor point();
    static SamplerDescriptor linear();
    static SamplerDescriptor anisotropic();

    static void createSamplers(Device& device, float anisotropy);

   private:
    static SamplerDescriptor createSampler(Device& device, VkFilter filter,
                                           bool anisotropic, float anisotropy);

    static SamplerDescriptor point_sampler;
    static SamplerDescriptor linear_sampler;
    static SamplerDescriptor anisotropic_sampler;
};

}  // namespace Graphics