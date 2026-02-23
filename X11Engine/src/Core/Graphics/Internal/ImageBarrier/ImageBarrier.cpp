#include "ImageBarrier.h"

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

ImageBarrier::ImageBarrier(VkImage image, VkImageLayout new_layout)
    : image(image),
      old_layout(VK_IMAGE_LAYOUT_UNDEFINED),
      new_layout(new_layout),
      src_stage_mask(VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT),
      dst_stage_mask(VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT),
      src_access_mask(VK_ACCESS_2_MEMORY_WRITE_BIT),
      dst_access_mask(VK_ACCESS_2_MEMORY_WRITE_BIT |
                      VK_ACCESS_2_MEMORY_READ_BIT) {}

ImageBarrier::ImageBarrier(const Texture& image, VkImageLayout new_layout)
    : ImageBarrier(image.image, new_layout) {
    old_layout = image.layout;
}

VkImageMemoryBarrier2 ImageBarrier::create() const {
    VkImageMemoryBarrier2 barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    barrier.image = image;
    barrier.srcStageMask = src_stage_mask;
    barrier.srcAccessMask = src_access_mask;
    barrier.dstStageMask = dst_stage_mask;
    barrier.dstAccessMask = dst_access_mask;
    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    return barrier;
}

}  // namespace Graphics::Internal