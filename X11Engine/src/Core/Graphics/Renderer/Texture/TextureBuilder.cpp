#include "TextureBuilder.h"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"
#include "InternalTexture.h"
#include "Semaphore.h"
#include "Texture.h"

namespace Graphics {

TextureBuilder::TextureBuilder(Texture::Format format, uint32_t width,
                               uint32_t height)
    : data(nullptr),
      format(format),
      width(width),
      height(height),
      shader_resource(false),
      render_target(false),
      depth_stencil(false),
      cpu_writable(false),
      is_copy_target(false),
      is_copy_source(false) {}

TextureBuilder& TextureBuilder::isShaderResource() {
    shader_resource = true;
    return *this;
}

TextureBuilder& TextureBuilder::isRenderTarget() {
    render_target = true;
    return *this;
}

TextureBuilder& TextureBuilder::isDepthStencil() {
    depth_stencil = true;
    return *this;
}

TextureBuilder& TextureBuilder::isCPUWritable() {
    cpu_writable = true;
    return *this;
}

TextureBuilder& TextureBuilder::isCopySource() {
    is_copy_source = true;
    return *this;
}

TextureBuilder& TextureBuilder::isCopyDestination() {
    is_copy_target = true;
    return *this;
}

static VkFormat translateFormat(Texture::Format format) {
    switch (format) {
        case Texture::Format::RGBA8:
            return VK_FORMAT_R8G8B8A8_UNORM;
        case Texture::Format::RGBA8_SRGB:
            return VK_FORMAT_R8G8B8A8_SRGB;
    }

    throw;
}

Result<Texture, TextureError> TextureBuilder::create() {
    auto native_format = translateFormat(format);

    VkImageCreateInfo image_info = {};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.pNext = nullptr;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent = {.width = width, .height = height, .depth = 1};
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.format = native_format;
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (shader_resource) image_info.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if (render_target) image_info.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (depth_stencil)
        image_info.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    if (is_copy_source) image_info.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    if (is_copy_target) image_info.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

    VkImage image;
    VmaAllocation allocation;
    vmaCreateImage(Resources::get().getAllocator(), &image_info, &alloc_info,
                   &image, &allocation, nullptr);

    auto internal = Internal::Texture{};
    internal.image = image;
    internal.allocation = allocation;
    internal.layout = VK_IMAGE_LAYOUT_UNDEFINED;
    internal.format = native_format;
    internal.width = width;
    internal.height = height;

    return Texture(internal);
}

}  // namespace Graphics
