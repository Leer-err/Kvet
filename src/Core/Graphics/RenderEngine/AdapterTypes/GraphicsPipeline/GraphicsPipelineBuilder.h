#pragma once

#include <vulkan/vulkan.h>

#include <optional>
#include <string>

#include "EngineData.h"
#include "GraphicsPipeline.h"

// #include "InputLayout.h"
#include "InputLayout.h"
#include "Result.h"
#include "Shader.h"
// #include "RenderTarget.h"

namespace Graphics {

class GraphicsPipelineBuilder {
   public:
    enum class Error { ShaderNotBuilt, VertexInputTypeNotSupported };

    GraphicsPipelineBuilder(const EngineData& engine_data,
                            const std::string& vertex_shader_filename,
                            const std::string& vertex_shader_entrypoint,
                            const std::string& pixel_shader_filename,
                            const std::string& pixel_shader_entrypoint);

    GraphicsPipelineBuilder& setRasterizer(
        VkPipelineRasterizationStateCreateInfo rasterizer);

    GraphicsPipelineBuilder& setRenderTargetFormat(VkFormat format);

    Result<GraphicsPipeline, Error> create();

   private:
    Result<Shader, GraphicsPipelineBuilder::Error> createShader(
        const std::string& filename, const std::string& entrypoint,
        VkShaderStageFlagBits stage);

    EngineData engine_data;

    Shader vertex_shader;
    Shader pixel_shader;

    VkFormat render_target_format;
    VkPipelineRasterizationStateCreateInfo rasterizer;

    std::optional<Error> error;
};

}  // namespace Graphics