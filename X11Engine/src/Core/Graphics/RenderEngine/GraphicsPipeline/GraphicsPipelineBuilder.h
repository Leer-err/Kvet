#pragma once

// #include "DepthStencil.h"
#include <vulkan/vulkan_core.h>

#include <optional>

#include "GraphicsPipeline.h"


// #include "InputLayout.h"
#include "InputLayout.h"
#include "Shader.h"
// #include "RenderTarget.h"

namespace Graphics {

class GraphicsPipelineBuilder {
   public:
    GraphicsPipelineBuilder(const Shader& vertex_shader,
                            const Shader& pixel_shader);
    GraphicsPipelineBuilder(const InputLayout& input_layout,
                            const Shader& vertex_shader,
                            const Shader& pixel_shader);

    GraphicsPipelineBuilder& setRasterizer(
        VkPipelineRasterizationStateCreateInfo rasterizer);

    GraphicsPipeline create();

   private:
    void createPipelineLayout(GraphicsPipeline& pipeline,
                              const InputLayout& layout);

    bool default_render_target;
    bool has_depth_stencil;

    std::optional<InputLayout> input_layout;

    Shader vertex_shader;
    Shader pixel_shader;

    VkPipelineRasterizationStateCreateInfo rasterizer;
};

}  // namespace Graphics