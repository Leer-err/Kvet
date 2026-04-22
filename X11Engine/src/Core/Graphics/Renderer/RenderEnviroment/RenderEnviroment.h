#pragma once

#include <optional>

#include "RenderTarget.h"
#include "Vector4.h"


namespace Graphics {

struct RenderEnviroment {
    std::optional<RenderTarget> render_target;
    bool clear_render_target;
    Vector4 render_target_clear_value;

    // std::optional<DepthStencil> depth_stencil;
    // bool clear_depth_stencil;
    // float clear_depth;
    // int clear_stencil;
};

}  // namespace Graphics