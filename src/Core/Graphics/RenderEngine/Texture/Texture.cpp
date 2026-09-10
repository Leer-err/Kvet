#include "Texture.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <cassert>

namespace Graphics {

Result<Texture, TextureError> Texture::create(
    VkDevice device, VmaAllocator allocator,
    const VkImageCreateInfo& image_info,
    const VmaAllocationCreateInfo& alloc_info,
    IndexAllocator<TextureDescriptor>& index_allocator) {
    VkImage image;
    VmaAllocation allocation;
    VkResult result = vmaCreateImage(allocator, &image_info, &alloc_info,
                                     &image, &allocation, nullptr);
    if (result != VK_SUCCESS) assert(false);

    VkImageViewCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = image;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = image_info.format;
    info.subresourceRange = {.levelCount = 1, .layerCount = 1};
    if (image_info.format == VK_FORMAT_D24_UNORM_S8_UINT ||
        image_info.format == VK_FORMAT_D32_SFLOAT_S8_UINT)
        info.subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    else
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    VkImageView view;
    vkCreateImageView(device, &info, nullptr, &view);
    auto descriptor = index_allocator.allocate();
    if (descriptor.has_value() == false) return TextureError::OutOfDescriptors;

    auto allocated_image =
        AllocatedTexture{image, allocation, view, descriptor.value()};
    return Texture(allocator, allocated_image, image_info, alloc_info,
                   index_allocator);
}

Texture::Texture(VmaAllocator allocator,
                 const AllocatedTexture& allocated_image,
                 const VkImageCreateInfo& image_info,
                 const VmaAllocationCreateInfo& alloc_info,
                 IndexAllocator<TextureDescriptor>& index_allocator)
    : allocator(allocator),
      texture(allocated_image.texture),
      allocation(allocated_image.allocation),
      layout(image_info.initialLayout),
      view(allocated_image.view),
      descriptor(allocated_image.descriptor),
      index_allocator(index_allocator),
      format(image_info.format),
      width(image_info.extent.width),
      height(image_info.extent.height) {}

Texture::~Texture() {
    if (texture == VK_NULL_HANDLE) return;

    index_allocator.free(descriptor);
    vmaDestroyImage(allocator, texture, allocation);
}

Texture& Texture::operator=(Texture&& other) noexcept {
    allocator = other.allocator;
    texture = other.texture;
    allocation = other.allocation;
    layout = other.layout;
    view = other.view;
    descriptor = other.descriptor;
    index_allocator = other.index_allocator;
    format = other.format;
    width = other.width;
    height = other.height;

    other.texture = VK_NULL_HANDLE;

    return *this;
}

Texture::Texture(Texture&& other) noexcept
    : allocator(other.allocator),
      texture(other.texture),
      allocation(other.allocation),
      layout(other.layout),
      view(other.view),
      descriptor(other.descriptor),
      index_allocator(other.index_allocator),
      format(other.format),
      width(other.width),
      height(other.height) {
    other.texture = VK_NULL_HANDLE;
}

uint32_t Texture::getWidth() const { return width; }

uint32_t Texture::getHeight() const { return height; }

VkImage Texture::getImage() const { return texture; }

VkFormat Texture::getFormat() const { return format; }

VkImageLayout Texture::getLayout() const { return layout; }

VkImageView Texture::getView() const { return view; }

TextureDescriptor Texture::getDescriptor() const { return descriptor; }

VkImageMemoryBarrier2 Texture::createBarrier(VkImageLayout new_layout,
                                             VkPipelineStageFlags2 src_stages,
                                             VkAccessFlags2 src_access,
                                             VkPipelineStageFlags2 dst_stages,
                                             VkAccessFlags2 dst_access) {
    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    if (format == VK_FORMAT_D24_UNORM_S8_UINT ||
        format == VK_FORMAT_D32_SFLOAT_S8_UINT)
        aspect = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

    auto barrier = VkImageMemoryBarrier2{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    barrier.image = texture;
    barrier.srcStageMask = src_stages;
    barrier.srcAccessMask = src_access;
    barrier.dstStageMask = dst_stages;
    barrier.dstAccessMask = dst_access;
    barrier.oldLayout = layout;
    barrier.newLayout = new_layout;
    barrier.subresourceRange.aspectMask = aspect;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    layout = new_layout;

    return barrier;
}

}  // namespace Graphics