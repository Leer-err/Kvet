#pragma once

#include <vulkan/vulkan.h>

#include "EngineData.h"

namespace Graphics {

class Sampler {
   public:
    static VkSampler point(const EngineData& engine_data);
    static VkSampler linear(const EngineData& engine_data);
    static VkSampler anisotropic(const EngineData& engine_data,
                                 float anisotropy);

   private:
    static VkSampler createSampler(const EngineData& engine_data,
                                   VkFilter filter, bool anisotropic,
                                   float anisotropy);
};

}  // namespace Graphics