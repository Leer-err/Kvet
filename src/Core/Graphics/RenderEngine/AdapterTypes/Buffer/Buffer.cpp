#include "Buffer.h"

#include <vulkan/vulkan.h>

namespace Graphics {

VkBufferMemoryBarrier2 Buffer::createBarrier(VkPipelineStageFlags2 src_stages,
                                             VkAccessFlags2 src_access,
                                             VkPipelineStageFlags2 dst_stages,
                                             VkAccessFlags2 dst_access) {
    VkBufferMemoryBarrier2 barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;
    barrier.buffer = buffer;
    barrier.srcStageMask = src_stages;
    barrier.srcAccessMask = src_access;
    barrier.dstStageMask = dst_stages;
    barrier.dstAccessMask = dst_stages;
    barrier.size = VK_WHOLE_SIZE;

    return barrier;
}

}  // namespace Graphics