#include "Shader.h"

#include <memory>

#include "InternalShader.h"

namespace Graphics {

Shader::Shader(Internal::Shader&& shader)
    : shader(std::make_unique<Internal::Shader>(std::move(shader))) {}

Internal::Shader* Shader::getInternal() const { return shader.get(); }

}  // namespace Graphics