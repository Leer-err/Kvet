#pragma once

// #include "DepthStencil.h"
// #include "InputLayout.h"
// #include "Rasterizer.h"
// #include "RenderTarget.h"

// namespace Graphics = Engine::Graphics;

class GraphicsPipeline {
    friend class GraphicsPipelineBuilder;

   public:
    GraphicsPipeline();
    ~GraphicsPipeline();

    GraphicsPipeline(GraphicsPipeline&&);
    GraphicsPipeline& operator=(GraphicsPipeline&&);

   protected:
    GraphicsPipeline(Internal::Pipeline&& buffer);
    Internal::Pipeline* getInternal() const;

   private:
    std::unique_ptr<Internal::Pipeline> pipeline;
};