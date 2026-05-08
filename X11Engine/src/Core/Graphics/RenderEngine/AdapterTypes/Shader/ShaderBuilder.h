#pragma once

#include <vulkan/vulkan.h>

#include <string>

#include "EngineData.h"
#include "Result.h"
#include "Shader.h"
#include "ShaderError.h"

namespace Graphics {

class ShaderBuilder {
   public:
    ShaderBuilder(const EngineData& engine_data, const std::string& filename,
                  const std::string& entrypoint, VkShaderStageFlagBits stage);

    Result<Shader, ShaderError> create();

   private:
    EngineData engine_data;

    std::string filename;
    std::string entrypoint;
    VkShaderStageFlagBits stage;
};

}  // namespace Graphics