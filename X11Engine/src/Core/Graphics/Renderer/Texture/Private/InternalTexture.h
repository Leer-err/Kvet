#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

#include <cstddef>

namespace Graphics::Internal {

struct Texture {
    void destroy();

    VkImageMemoryBarrier2 createBarrier(VkImageLayout new_layout,
                                        VkPipelineStageFlags2 src_stages,
                                        VkAccessFlags2 src_access,
                                        VkPipelineStageFlags2 dst_stages,
                                        VkAccessFlags2 dst_access) const;

    VkImage image;
    VmaAllocation allocation;
    VkImageLayout layout;

    VkFormat format;
    size_t width;
    size_t height;
};

}  // namespace Graphics::Internal