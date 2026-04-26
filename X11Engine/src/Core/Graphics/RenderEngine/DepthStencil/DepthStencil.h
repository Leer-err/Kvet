#pragma once

#include <vulkan/vulkan_core.h>

#include <cstdint>

#include "Image.h"

namespace Graphics {

class DepthStencil {
    static DepthStencil create(const Image& image);

    void destroy();

    VkImageView depth_stencil;
    uint32_t width;
    uint32_t height;
};

}  // namespace Graphics