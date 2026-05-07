#include "RenderTarget.h"

#include <vulkan/vulkan.h>

namespace Graphics {

RenderTarget RenderTarget::create(const APIData& api_data, const Image& image) {
    VkImageViewCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = image.image;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = image.format;
    info.subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                             .levelCount = 1,
                             .layerCount = 1};

    VkImageView view;
    vkCreateImageView(api_data.device, &info, nullptr, &view);

    RenderTarget render_target = {};
    render_target.render_target = view;
    render_target.width = image.width;
    render_target.height = image.height;
    return render_target;
}

void RenderTarget::destroy() {
    vkDestroyImageView(api_data.device, render_target, nullptr);
}

}  // namespace Graphics