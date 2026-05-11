#include "ShaderBuilder.h"

#include <vulkan/vulkan.h>

#include <optional>

#include "Shader.h"
#include "ShaderError.h"
#include "ShaderRegistry.h"

namespace Graphics {

ShaderBuilder::ShaderBuilder(const EngineData& engine_data,
                             const std::string& filename,
                             const std::string& entrypoint,
                             VkShaderStageFlagBits stage)
    : engine_data(engine_data),
      filename(filename),
      entrypoint(entrypoint),
      stage(stage) {}

Result<Shader, ShaderError> ShaderBuilder::create() {
    auto module = engine_data.shader_registry.getModule(filename);
    if (module == std::nullopt) return ShaderError::NotFound;

    auto result = Shader{};
    result.shader = *module;
    result.stage = stage;
    result.entrypoint = entrypoint;
    result.filename = filename;

    return result;
}

}  // namespace Graphics