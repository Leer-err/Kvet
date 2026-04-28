#include "Sampler.h"

#include <vulkan/vulkan.h>

#include "GraphicsResources.h"

namespace Graphics {

Sampler Sampler::create() {
    auto device = Resources::get().getDevice();

    VkSamplerCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.magFilter = VK_FILTER_LINEAR;

    Sampler sampler = {};
    vkCreateSampler(device, &info, nullptr, &sampler.sampler);

    return sampler;
}

VkSamplerCreateInfo createSampler(VkFilter filter, bool anisotropic,
                                  float anisotropy) {
    VkSamplerCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.magFilter = filter;
    info.magFilter = filter;
}

}  // namespace Graphics