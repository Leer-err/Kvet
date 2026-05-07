#pragma once

#include <vulkan/vulkan_core.h>

#include "Buffer.h"
#include "BufferBuilder.h"
#include "CommandPool.h"
#include "DeviceProperties.h"
#include "GraphicsPipeline.h"
#include "Image.h"
#include "Semaphore.h"
#include "VkBootstrap.h"

namespace Graphics {

class Device {
   public:
    Device(vkb::Device device, VmaAllocator allocator);

    Result<Image, ImageError> createImage(
        const VkImageCreateInfo& image_info,
        const VmaAllocationCreateInfo& alloc_info) const;

    Result<Buffer, BufferError> createBuffer(
        const VkBufferCreateInfo& buffer_info,
        const VmaAllocationCreateInfo& alloc_info) const;

    void* map(const Buffer& buffer) const;
    void unmap(const Buffer& buffer) const;

    VkCommandPool createCommandPool(uint32_t queue_index) const;
    void resetCommandPool(const CommandPool& command_pool) const;
    VkCommandBuffer createCommandBuffer(VkCommandPool pool) const;

    VkImageView createDepthStencil(const Image& image) const;
    VkImageView createRenderTarget(const Image& image) const;

    DescriptorLayout getDescriptorLayout() const;

    DeviceProperties getDeviceProperties() const;

    VkFence createFence(bool is_signaled) const;
    void waitFence(VkFence fence) const;
    void resetFence(VkFence fence) const;

    GraphicsPipeline createGraphicsPipeline(
        const VkGraphicsPipelineCreateInfo& pipeline_info) const;

    VkSampler createSampler(const VkSamplerCreateInfo& sampler_info) const;

    Semaphore createSemaphore(const VkSamplerCreateInfo& sampler_info) const;

   private:
    vkb::Device device;

    VmaAllocator allocator;
};

}  // namespace Graphics