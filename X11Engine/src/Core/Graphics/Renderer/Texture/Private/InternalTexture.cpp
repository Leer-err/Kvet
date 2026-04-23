#include "InternalTexture.h"

#include <vulkan/vulkan_core.h>

#include "GraphicsResources.h"

namespace Graphics::Internal {

void Texture::destroy() {
    Resources::get().getAllocator();

    vmaDestroyImage(Resources::get().getAllocator(), image, allocation);
}

VkImageMemoryBarrier2 Texture::createBarrier(VkImageLayout new_layout,
                                             VkPipelineStageFlags2 src_stages,
                                             VkAccessFlags2 src_access,
                                             VkPipelineStageFlags2 dst_stages,
                                             VkAccessFlags2 dst_access) const {
    VkImageMemoryBarrier2 barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    barrier.image = image;
    barrier.srcStageMask = src_stages;
    barrier.srcAccessMask = src_access;
    barrier.dstStageMask = dst_stages;
    barrier.dstAccessMask = dst_access;
    barrier.oldLayout = layout;
    barrier.newLayout = new_layout;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    return barrier;
}

}  // namespace Graphics::Internal