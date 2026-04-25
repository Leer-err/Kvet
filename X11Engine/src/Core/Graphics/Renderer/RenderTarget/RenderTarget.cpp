#include "RenderTarget.h"

#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"

namespace Graphics {

RenderTarget RenderTarget::create(const Image& image) {
    auto device = Resources::get().getDevice();

    VkImageViewCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = image.image;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = image.format;
    info.subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                             .levelCount = 1,
                             .layerCount = 1};

    VkImageView view;
    vkCreateImageView(device, &info, nullptr, &view);

    RenderTarget render_target = {};
    render_target.render_target = view;
    render_target.width = image.width;
    render_target.height = image.height;
    return render_target;
}

void RenderTarget::destroy() {
    auto device = Resources::get().getDevice();

    vkDestroyImageView(device, render_target, nullptr);
}

}  // namespace Graphics