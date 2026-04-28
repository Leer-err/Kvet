#pragma once

#include <vulkan/vulkan.h>

namespace Graphics {

class Sampler {
   public:
    static Sampler point();
    static Sampler linear();
    static Sampler anisotropic(float anisotropy);

    void destroy();

    VkSampler sampler;

   private:
    VkSamplerCreateInfo createSampler(VkFilter filter, bool anisotropic,
                                      float anisotropy);
};

}  // namespace Graphics