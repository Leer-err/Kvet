#pragma once

// #include "DepthStencil.h"
#include "GraphicsPipeline.h"
// #include "InputLayout.h"
#include "InputLayout.h"
#include "Shader.h"
// #include "RenderTarget.h"

namespace Graphics {

class GraphicsPipelineBuilder {
   public:
    // GraphicsPipelineBuilder(InputLayout input_layout,
    //                         VertexShader vertex_shader,
    //                         PixelShader pixel_shader);
    GraphicsPipelineBuilder(const InputLayout& input_layout,
                            const Shader& vertex_shader,
                            const Shader& pixel_shader);

    // GraphicsPipelineBuilder& setRenderTarget(RenderTarget render_target);
    // GraphicsPipelineBuilder& setDepthStencilBuffer(
    // DepthStencil depth_stencil_buffer);

    // GraphicsPipelineBuilder& setRasterizerState(
    // Engine::Graphics::Rasterizer rasterizer);

    GraphicsPipeline create();

   private:
    bool default_render_target;
    bool has_depth_stencil;

    const InputLayout& input_layout;

    const Shader& vertex_shader;
    const Shader& pixel_shader;

    // Engine::Graphics::Rasterizer rasterizer;
};

}  // namespace Graphics