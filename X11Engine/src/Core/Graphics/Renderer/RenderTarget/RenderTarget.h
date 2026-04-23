#pragma once

#include <memory>

#include "GraphicsInternalsForward.h"

namespace Graphics {

class Texture;

class RenderTarget {
   public:
    RenderTarget() = default;

    RenderTarget(const Internal::RenderTarget& render_target);
    Internal::RenderTarget* getInternal() const;

   private:
    std::shared_ptr<Internal::WrappedRenderTarget> render_target;
};

}  // namespace Graphics