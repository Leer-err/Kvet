#include "TextureView.h"

#include <vulkan/vulkan.h>

#include "GraphicsResources.h"
#include "Image.h"

namespace Graphics {

TextureView TextureView::create(const Image& image) {
    auto device = Graphics::Resources::get().getDevice();

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

    TextureView resource = {};
    resource.shader_resource = view;
    resource.width = static_cast<uint32_t>(image.width);
    resource.height = static_cast<uint32_t>(image.height);
    return resource;
}

void TextureView::destroy() {
    auto device = Graphics::Resources::get().getDevice();

    vkDestroyImageView(device, shader_resource, nullptr);
}

}  // namespace Graphics