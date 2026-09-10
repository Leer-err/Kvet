#pragma once

#include <VkBootstrap.h>
#include <vulkan/vulkan.h>

#include <cstddef>
#include <cstdint>
#include <tracy/TracyVulkan.hpp>

#include "DescriptorLayout.h"
#include "Descriptors.h"
#include "DeviceProperties.h"
#include "GraphicsPipeline.h"
#include "Handles.h"
#include "Logger.h"
#include "MeshRegistry.h"
#include "PoolAllocator.h"
#include "Queue.h"
#include "Registries.h"
#include "Result.h"
#include "Semaphore.h"

namespace Graphics {

struct CommandBuffer;

class Device {
   public:
    Device(const vkb::Instance& instance, const vkb::Device& device,
           VmaAllocator allocator);
    ~Device();

    vkb::Swapchain createSwapChain(VkSurfaceFormatKHR format,
                                   VkPresentModeKHR present_mode,
                                   size_t image_count, VkImageUsageFlags flags);

    Result<TextureHandle, TextureError> createTexture(
        const VkImageCreateInfo& image_info,
        const VmaAllocationCreateInfo& alloc_info);

    Result<BufferHandle, BufferError> createBuffer(
        const VkBufferCreateInfo& buffer_info,
        const VmaAllocationCreateInfo& alloc_info, bool is_chained);

    VkCommandPool createCommandPool(uint32_t queue_index);
    void resetCommandPool(VkCommandPool pool) const;
    VkCommandBuffer createCommandBuffer(VkCommandPool pool);

    DescriptorLayout getDescriptorLayout() const;
    DeviceProperties getDeviceProperties() const;

    VkFence createFence(bool is_signaled);
    void waitFence(VkFence fence);
    void resetFence(VkFence fence) const;

    VkShaderModule createShader(const uint32_t* shader_data,
                                size_t shader_data_size);

    VkPipelineLayout createPipelineLayout(
        const std::vector<size_t>& push_constant_ranges);
    GraphicsPipeline createGraphicsPipeline(
        const VkGraphicsPipelineCreateInfo& pipeline_info);

    SamplerDescriptor createSampler(const VkSamplerCreateInfo& sampler_info);

    Semaphore createSemaphore();

    void waitIdle() const;

    VkInstance getInstance() const;
    VkDevice getDevice() const;
    VkPhysicalDevice getPhysicalDevice() const;
    BufferRegistry& getBufferRegistry();

    TracyVkCtx createTracingContext(const Queue& queue,
                                    const CommandBuffer& command_buffer) const;

   private:
    BufferHandle createDescriptorBuffer(size_t set_size, size_t alignment);

    static DescriptorLayout createDescriptorLayout(VkDevice device);

    void writeTextureDescriptor(TextureDescriptor index,
                                VkImageView descriptor);
    void writeSamplerDescriptor(SamplerDescriptor index, VkSampler descriptor);

    vkb::Instance instance;
    vkb::Device device;

    VmaAllocator allocator;

    PoolAllocator buffer_allocator;
    BufferRegistry buffer_registry;

    PoolAllocator texture_allocator;
    TextureRegistry texture_registry;

    DescriptorLayout descriptor_layout;
    DeviceProperties properties;

    BufferHandle descriptors;
    IndexAllocator<TextureDescriptor> texture_descriptor_allocator;
    IndexAllocator<SamplerDescriptor> sampler_descriptor_allocator;

    Logger logger;
};

}  // namespace Graphics