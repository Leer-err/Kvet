#include "ShaderBuilder.h"

#include <vulkan/vulkan_core.h>

#include <optional>

#include "Shader.h"
#include "ShaderError.h"
#include "ShaderRegistry.h"

namespace Graphics {

ShaderBuilder::ShaderBuilder(const std::string& filename,
                             const std::string& entrypoint,
                             VkShaderStageFlagBits stage)
    : filename(filename),
      entrypoint(entrypoint),
      stage(stage),
      constant_range_size(0) {}

ShaderBuilder& ShaderBuilder::withConstants(size_t constant_range_size) {
    this->constant_range_size = constant_range_size;

    return *this;
}

Result<Shader, ShaderError> ShaderBuilder::create() {
    auto module = ShaderRegistry::get().getModule(filename);
    if (module == std::nullopt) return ShaderError::NotFound;

    auto result = Shader{};
    result.shader = *module;
    result.stage = stage;
    result.entrypoint = entrypoint;
    result.constant_range_size = constant_range_size;

    return result;
}

}  // namespace Graphics