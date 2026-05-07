#pragma once

#include <vulkan/vulkan.h>

#include "APIData.h"

namespace Graphics {

class Sampler {
   public:
    static Sampler point(const APIData& api_data);
    static Sampler linear(const APIData& api_data);
    static Sampler anisotropic(const APIData& api_data, float anisotropy);

    void destroy();

    VkSampler sampler;

   private:
    static VkSampler createSampler(const APIData& api_data, VkFilter filter,
                                   bool anisotropic, float anisotropy);
};

}  // namespace Graphics