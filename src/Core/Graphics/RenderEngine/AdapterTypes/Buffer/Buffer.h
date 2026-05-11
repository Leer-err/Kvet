#pragma once

#include <stddef.h>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

namespace Graphics {

enum class BufferError { NoDataForImmutableResource, WriteFromGPUAndCPU };

struct Buffer {
    VkBufferMemoryBarrier2 createBarrier(VkPipelineStageFlags2 src_stages,
                                         VkAccessFlags2 src_access,
                                         VkPipelineStageFlags2 dst_stages,
                                         VkAccessFlags2 dst_access);

    VkBuffer buffer;
    VmaAllocation allocation;

    VkDeviceAddress device_address;
    size_t size;
};

}  // namespace Graphics
