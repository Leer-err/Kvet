#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

struct RenderTarget {
    void destroy();

    VkImageView render_target;
    VkFormat format;
};

}  // namespace Graphics::Internal