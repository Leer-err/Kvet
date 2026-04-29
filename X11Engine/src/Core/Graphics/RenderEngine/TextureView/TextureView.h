#pragma once

#include <vulkan/vulkan.h>

#include "Image.h"

namespace Graphics {

struct TextureView {
    static TextureView create(const Image& image);

    void destroy();

    VkImageView shader_resource;
    uint32_t width;
    uint32_t height;
};

}  // namespace Graphics