#include "ImageBuilder.h"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include "GraphicsResources.h"
#include "Image.h"

namespace Graphics {

ImageBuilder::ImageBuilder(VkFormat format, uint32_t width, uint32_t height)
    : format(format),
      width(width),
      height(height),
      shader_resource(false),
      render_target(false),
      depth_stencil(false),
      cpu_writable(false),
      is_copy_target(false),
      is_copy_source(false) {}

ImageBuilder& ImageBuilder::isShaderResource() {
    shader_resource = true;
    return *this;
}

ImageBuilder& ImageBuilder::isRenderTarget() {
    render_target = true;
    return *this;
}

ImageBuilder& ImageBuilder::isDepthStencil() {
    depth_stencil = true;
    return *this;
}

ImageBuilder& ImageBuilder::isCPUWritable() {
    cpu_writable = true;
    return *this;
}

ImageBuilder& ImageBuilder::isCopySource() {
    is_copy_source = true;
    return *this;
}

ImageBuilder& ImageBuilder::isCopyDestination() {
    is_copy_target = true;
    return *this;
}

Result<Image, ImageError> ImageBuilder::create() {
    VkImageCreateInfo image_info = {};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.pNext = nullptr;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent = {.width = width, .height = height, .depth = 1};
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.format = format;
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

    auto result = Image{};
    result.image = image;
    result.allocation = allocation;
    result.layout = VK_IMAGE_LAYOUT_UNDEFINED;
    result.format = format;
    result.width = width;
    result.height = height;

    return result;
}

}  // namespace Graphics
