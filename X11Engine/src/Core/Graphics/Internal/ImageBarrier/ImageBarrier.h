#pragma once

#include <vulkan/vulkan_core.h>

#include "InternalTexture.h"

namespace Graphics::Internal {

struct ImageBarrier {
    ImageBarrier(VkImage image, VkImageLayout new_layout);
    ImageBarrier(const Texture& image, VkImageLayout new_layout);

    VkImageMemoryBarrier2 create() const;

    VkImage image;
    VkImageLayout old_layout;
    VkImageLayout new_layout;
    VkPipelineStageFlags src_stage_mask;
    VkPipelineStageFlags dst_stage_mask;
    VkAccessFlags src_access_mask;
    VkAccessFlags dst_access_mask;
};

}  // namespace Graphics::Internal