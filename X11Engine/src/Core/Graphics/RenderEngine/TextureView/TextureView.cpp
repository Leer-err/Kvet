#include "TextureView.h"

#include <vulkan/vulkan.h>

#include "GraphicsResources.h"

namespace Graphics {

TextureView TextureView::create(const Image& image) {
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

    TextureView texture_view = {};
    texture_view.view = view;
    texture_view.width = image.width;
    texture_view.height = image.height;
    return texture_view;
}

void TextureView::destroy() {
    auto device = Resources::get().getDevice();

    vkDestroyImageView(device, view, nullptr);
}

}  // namespace Graphics