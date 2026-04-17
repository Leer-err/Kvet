#pragma once

#include <string>

#include "Result.h"
#include "ShaderError.h"
#include "ShaderStage.h"

namespace Graphics {

class Shader;

class ShaderBuilder {
   public:
    ShaderBuilder(const std::string& filename, const std::string& entrypoint,
                  ShaderStage stage);

    Result<Shader, ShaderError> create();

   private:
    std::string filename;
    std::string entrypoint;
    ShaderStage stage;
};

}  // namespace Graphics