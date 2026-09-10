#include "Sampler.h"

#include <vulkan/vulkan.h>

#include "Device.h"

namespace Graphics {

SamplerDescriptor Sampler::point_sampler = 0;

SamplerDescriptor Sampler::linear_sampler = 0;

SamplerDescriptor Sampler::anisotropic_sampler = 0;

SamplerDescriptor Sampler::point() { return point_sampler; }

SamplerDescriptor Sampler::linear() { return linear_sampler; }

SamplerDescriptor Sampler::anisotropic() { return anisotropic_sampler; }

void Sampler::createSamplers(Device& device, float anisotropy) {
    point_sampler = createSampler(device, VK_FILTER_NEAREST, false, 0);
    linear_sampler = createSampler(device, VK_FILTER_LINEAR, false, 0);
    // anisotropic_sampler =
    //     createSampler(device, VK_FILTER_LINEAR, true, anisotropy);
}

SamplerDescriptor Sampler::createSampler(Device& device, VkFilter filter,
                                         bool anisotropic, float anisotropy) {
    VkSamplerMipmapMode mipmap_mode;
    switch (filter) {
        case VK_FILTER_NEAREST:
            mipmap_mode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
            break;
        case VK_FILTER_LINEAR:
            mipmap_mode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            break;
        default:
            return 0;
    }

    VkSamplerCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.magFilter = filter;
    info.magFilter = filter;
    info.mipmapMode = mipmap_mode;
    info.anisotropyEnable = anisotropic;
    info.maxAnisotropy = anisotropy;
    info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

    return device.createSampler(info);
}

}  // namespace Graphics