#pragma once

#include <memory>

#include "GraphicsInternalsForward.h"

namespace Graphics {

class GraphicsPipeline {
    friend class GraphicsPipelineBuilder;
    friend class Context;

   public:
    GraphicsPipeline() = default;

   protected:
    GraphicsPipeline(const Internal::Pipeline& pipeline);
    const Internal::Pipeline* getInternal() const;

   private:
    std::shared_ptr<Internal::WrappedPipeline> pipeline;
};

}  // namespace Graphics