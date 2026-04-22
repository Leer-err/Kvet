#pragma once

#include "RenderTarget.h"
#include "Vector4.h"

namespace Graphics {

class RenderEnviromentBuilder {
   public:
   private:
    bool clear_render_target;
    Vector4 render_target_clear_color;

    RenderTarget render_target;
};

}  // namespace Graphics