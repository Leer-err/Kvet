#pragma once

#include <vulkan/vulkan_core.h>

#include <string>

#include "Result.h"
#include "ShaderError.h"

namespace Graphics {

class Shader;

class ShaderBuilder {
   public:
    ShaderBuilder(const std::string& filename, const std::string& entrypoint,
                  VkShaderStageFlagBits stage);

    ShaderBuilder& withConstants(size_t constant_range_size);

    Result<Shader, ShaderError> create();

   private:
    std::string filename;
    std::string entrypoint;
    VkShaderStageFlagBits stage;
    size_t constant_range_size;
};

}  // namespace Graphics