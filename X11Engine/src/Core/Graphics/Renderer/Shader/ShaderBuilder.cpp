#include "ShaderBuilder.h"

#include <vulkan/vulkan_core.h>

#include <optional>

#include "InternalShader.h"
#include "Shader.h"
#include "ShaderError.h"
#include "ShaderRegistry.h"

namespace Graphics {

ShaderBuilder::ShaderBuilder(const std::string& filename,
                             const std::string& entrypoint, ShaderStage stage)
    : filename(filename), entrypoint(entrypoint), stage(stage) {}

Result<Shader, ShaderError> ShaderBuilder::create() {
    auto module = ShaderRegistry::get().getModule(filename);
    if (module == std::nullopt) return ShaderError::NotFound;

    VkShaderStageFlagBits vk_stage;

    switch (stage) {
        case ShaderStage::Vertex:
            vk_stage = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        case ShaderStage::Pixel:
            vk_stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    }

    auto internal = Internal::Shader{};
    internal.shader = *module;
    internal.stage = vk_stage;
    internal.entrypoint = entrypoint;

    return Shader(std::move(internal));
}

}  // namespace Graphics