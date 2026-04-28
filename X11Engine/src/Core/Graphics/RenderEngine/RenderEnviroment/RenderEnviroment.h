#pragma once

#include <vulkan/vulkan.h>

#include <optional>

#include "RenderTarget.h"

namespace Graphics {

struct RenderEnviroment {
    RenderTarget render_target;
    bool clear_render_target;
    VkClearValue render_target_clear_value;

    // std::optional<DepthStencil> depth_stencil;
    // bool clear_depth_stencil;
    // float clear_depth;
    // int clear_stencil;
};

}  // namespace Graphics