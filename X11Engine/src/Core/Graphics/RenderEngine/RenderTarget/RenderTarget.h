#pragma once

#include <vulkan/vulkan.h>

#include "Image.h"

namespace Graphics {

struct RenderTarget {
   public:
    static RenderTarget create(const APIData& api_data, const Image& image);

    void destroy();

    APIData api_data;
    VkImageView render_target;
    uint32_t width;
    uint32_t height;
};

}  // namespace Graphics