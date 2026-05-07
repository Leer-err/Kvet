#pragma once

#include <vulkan/vulkan.h>

namespace Graphics {

class DepthStencil {
    VkImageView depth_stencil;
};

}  // namespace Graphics