#include "DepthStencil.h"

#include <vulkan/vulkan.h>

#include "Image.h"

namespace Graphics {

DepthStencil DepthStencil::create(const APIData& api_data, const Image& image) {
    VkImageViewCreateInfo info = {};

    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = image.image;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = VK_FORMAT_D24_UNORM_S8_UINT;
    info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    info.subresourceRange.levelCount = 1;
    info.subresourceRange.layerCount = 1;

    DepthStencil result = {};
    result.width = image.width;
    result.height = image.height;
    result.device = api_data.device;
    vkCreateImageView(api_data.device, &info, nullptr, &result.depth_stencil);

    return result;
}

void DepthStencil::destroy() {
    vkDestroyImageView(device, depth_stencil, nullptr);
}

}  // namespace Graphics