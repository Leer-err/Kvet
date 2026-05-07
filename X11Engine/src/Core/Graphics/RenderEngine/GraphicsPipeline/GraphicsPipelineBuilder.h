#pragma once

// #include "DepthStencil.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <optional>
#include <string>

#include "APIData.h"
#include "DescriptorSet.h"
#include "GraphicsPipeline.h"

// #include "InputLayout.h"
#include "InputLayout.h"
#include "Result.h"
#include "Shader.h"
// #include "RenderTarget.h"

namespace Graphics {

class GraphicsPipelineBuilder {
   public:
    enum class Error { ShaderFileNotFound, VertexInputTypeNotSupported };

    GraphicsPipelineBuilder(const APIData& api_data,
                            const Shader& vertex_shader,
                            const Shader& pixel_shader,
                            VkDescriptorSetLayout descriptor_layout);
    GraphicsPipelineBuilder(const APIData& api_data,
                            const std::string& vertex_shader_filename,
                            const std::string& vertex_shader_entrypoint,
                            const std::string& pixel_shader_filename,
                            const std::string& pixel_shader_entrypoint,
                            VkDescriptorSetLayout descriptor_layout);

    GraphicsPipelineBuilder& setRasterizer(
        VkPipelineRasterizationStateCreateInfo rasterizer);

    GraphicsPipelineBuilder& setRenderTargetFormat(VkFormat format);

    Result<GraphicsPipeline, Error> create();

   private:
    void createPipelineLayout(GraphicsPipeline& pipeline,
                              const InputLayout& layout);

    Result<Shader, GraphicsPipelineBuilder::Error> createShader(
        const std::string& filename, const std::string& entrypoint,
        VkShaderStageFlagBits stage);

    const APIData& api_data;

    Shader vertex_shader;
    Shader pixel_shader;

    VkFormat render_target_format;
    VkDescriptorSetLayout descriptor_layout;
    VkPipelineRasterizationStateCreateInfo rasterizer;

    std::optional<Error> error;
};

}  // namespace Graphics