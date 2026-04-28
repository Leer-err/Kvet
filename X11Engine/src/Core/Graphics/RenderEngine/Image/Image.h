#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <cstddef>

namespace Graphics {

struct Image {
    void destroy();

    VkImageMemoryBarrier2 createBarrier(VkImageLayout new_layout,
                                        VkPipelineStageFlags2 src_stages,
                                        VkAccessFlags2 src_access,
                                        VkPipelineStageFlags2 dst_stages,
                                        VkAccessFlags2 dst_access);

    VkImage image;
    VmaAllocation allocation;
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;

    VkFormat format;
    size_t width;
    size_t height;
};

}  // namespace Graphics