#pragma once

#include <VkBootstrap.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <cstddef>
#include <cstdint>

#include "Buffer.h"
#include "DescriptorLayout.h"
#include "DeviceProperties.h"
#include "GraphicsPipeline.h"
#include "Image.h"
#include "Result.h"
#include "Semaphore.h"

namespace Graphics {

class Device {
   public:
    Device(vkb::Device device, VmaAllocator allocator);
    ~Device();

    vkb::Swapchain createSwapChain(VkSurfaceFormatKHR format,
                                   VkPresentModeKHR present_mode,
                                   size_t image_count,
                                   VkImageUsageFlags flags) const;

    Result<Image, ImageError> createImage(
        const VkImageCreateInfo& image_info,
        const VmaAllocationCreateInfo& alloc_info) const;

    Result<Buffer, BufferError> createBuffer(
        const VkBufferCreateInfo& buffer_info,
        const VmaAllocationCreateInfo& alloc_info) const;

    void* map(const Buffer& buffer) const;
    void unmap(const Buffer& buffer) const;

    VkCommandPool createCommandPool(uint32_t queue_index) const;
    void resetCommandPool(VkCommandPool pool) const;
    VkCommandBuffer createCommandBuffer(VkCommandPool pool) const;

    VkImageView createDepthStencil(const Image& image) const;
    VkImageView createRenderTarget(const Image& image) const;

    DescriptorLayout getDescriptorLayout() const;

    DeviceProperties getDeviceProperties() const;

    VkFence createFence(bool is_signaled) const;
    void waitFence(VkFence fence) const;
    void resetFence(VkFence fence) const;

    VkShaderModule createShader(const uint32_t* shader_data,
                                size_t shader_data_size) const;

    VkPipelineLayout createPipelineLayout(
        const VkGraphicsPipelineCreateInfo& pipeline_info) const;
    GraphicsPipeline createGraphicsPipeline(
        const VkGraphicsPipelineCreateInfo& pipeline_info) const;

    VkSampler createSampler(const VkSamplerCreateInfo& sampler_info) const;

    Semaphore createSemaphore() const;

    void writeDescriptor(const VkDescriptorGetInfoEXT& info,
                         size_t descriptor_size, void* dst) const;

    void waitIdle() const;

    VkDevice getHandle() const;

   private:
    void createDescriptorLayout();

    vkb::Device device;

    VmaAllocator allocator;

    DescriptorLayout descriptor_layout;
    DeviceProperties properties;
};

}  // namespace Graphics