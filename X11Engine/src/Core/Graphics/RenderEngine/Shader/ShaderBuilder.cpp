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
    : filename(filename), entrypoint(entrypoint), stage(stage) {}

Result<Shader, ShaderError> ShaderBuilder::create() {
    auto module = ShaderRegistry::get().getModule(filename);
    if (module == std::nullopt) return ShaderError::NotFound;

    auto result = Shader{};
    result.shader = *module;
    result.stage = stage;
    result.entrypoint = entrypoint;
    result.filename = filename;

    return result;
}

}  // namespace Graphics