#pragma once

#include <memory>

#include "GraphicsInternalsForward.h"

namespace Graphics {

class RenderTarget {
    friend class GraphicsPipelineBuilder;

   public:
   protected:
    RenderTarget(const Internal::RenderTarget& render_target);
    Internal::RenderTarget* getInternal() const;

   private:
    std::shared_ptr<Internal::WrappedRenderTarget> render_target;
};

}  // namespace Graphics