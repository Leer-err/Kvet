#include "Sampler.h"

#include <vulkan/vulkan.h>

namespace Graphics {

Sampler Sampler::point(const APIData& api_data) {
    Sampler sampler = {};
    sampler.sampler = createSampler(api_data, VK_FILTER_NEAREST, false, 0);

    return sampler;
}

Sampler Sampler::linear(const APIData& api_data) {
    Sampler sampler = {};
    sampler.sampler = createSampler(api_data, VK_FILTER_LINEAR, false, 0);

    return sampler;
}

Sampler Sampler::anisotropic(const APIData& api_data, float anisotropy) {
    Sampler sampler = {};
    sampler.sampler =
        createSampler(api_data, VK_FILTER_LINEAR, true, anisotropy);

    return sampler;
}

VkSampler Sampler::createSampler(const APIData& api_data, VkFilter filter,
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
            throw;
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

    VkSampler sampler = {};
    vkCreateSampler(api_data.device, &info, nullptr, &sampler);

    return sampler;
}

}  // namespace Graphics