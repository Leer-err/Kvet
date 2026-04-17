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

    VkPipelineShaderStageCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.module = *module;
    info.pName = entrypoint.c_str();

    switch (stage) {
        case ShaderStage::Vertex:
            info.stage = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        case ShaderStage::Pixel:
            info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    }

    auto internal = Internal::Shader{};
    internal.info = info;

    return Shader(std::move(internal));
}

}  // namespace Graphics