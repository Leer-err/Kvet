#include "Sampler.h"

#include <vulkan/vulkan.h>

#include "EngineData.h"

namespace Graphics {

VkSampler Sampler::point(const EngineData& engine_data) {
    return createSampler(engine_data, VK_FILTER_NEAREST, false, 0);
}

VkSampler Sampler::linear(const EngineData& engine_data) {
    return createSampler(engine_data, VK_FILTER_LINEAR, false, 0);
}

VkSampler Sampler::anisotropic(const EngineData& engine_data,
                               float anisotropy) {
    return createSampler(engine_data, VK_FILTER_LINEAR, true, anisotropy);
}

VkSampler Sampler::createSampler(const EngineData& engine_data, VkFilter filter,
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

    return engine_data.device.createSampler(info);
}

}  // namespace Graphics