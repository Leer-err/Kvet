#include "Device.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "Buffer.h"
#include "DeviceProperties.h"
#include "GraphicsPipeline.h"
#include "Image.h"

constexpr auto TEXTURE_BINDING_INDEX = 0;
constexpr auto SAMPLER_BINDING_INDEX = 1;

constexpr auto MAX_TEXTURE_DESCRIPTORS_COUNT = 1000;
constexpr auto MAX_SAMPLE_DESCRIPTORS_COUNT = 1000;

namespace Graphics {

Device::Device(vkb::Device device, VmaAllocator allocator)
    : device(device), allocator(allocator) {
    createDescriptorLayout();
    properties = DeviceProperties::readProperties(device.physical_device);
}

Device::~Device() {
    vmaDestroyAllocator(allocator);
    vkb::destroy_device(device);
}

vkb::Swapchain Device::createSwapChain(VkSurfaceFormatKHR format,
                                       VkPresentModeKHR present_mode,
                                       size_t image_count,
                                       VkImageUsageFlags flags) const {
    vkb::SwapchainBuilder swapchain_builder{device};

    swapchain_builder.set_desired_format(format);
    swapchain_builder.add_fallback_present_mode(present_mode);
    swapchain_builder.set_desired_min_image_count(image_count);
    swapchain_builder.set_image_usage_flags(flags);

    auto swap_ret = swapchain_builder.build();
    if (!swap_ret) {
    }

    return swap_ret.value();
}

Result<Image, ImageError> Device::createImage(
    const VkImageCreateInfo& image_info,
    const VmaAllocationCreateInfo& alloc_info) const {
    VkImage image;
    VmaAllocation allocation;
    vmaCreateImage(allocator, &image_info, &alloc_info, &image, &allocation,
                   nullptr);

    auto result = Image{};
    result.image = image;
    result.allocation = allocation;
    result.layout = VK_IMAGE_LAYOUT_UNDEFINED;
    result.format = image_info.format;
    result.width = image_info.extent.width;
    result.height = image_info.extent.height;

    return result;
}

Result<Buffer, BufferError> Device::createBuffer(
    const VkBufferCreateInfo& buffer_info,
    const VmaAllocationCreateInfo& alloc_info) const {
    Buffer buffer = {};

    vmaCreateBuffer(allocator, &buffer_info, &alloc_info, &buffer.buffer,
                    &buffer.allocation, nullptr);

    VkBufferDeviceAddressInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
    info.buffer = buffer.buffer;
    buffer.device_address = vkGetBufferDeviceAddress(device, &info);

    buffer.size = buffer_info.size;
    return buffer;
}

void* Device::map(const Buffer& buffer) const {
    void* data;
    vmaMapMemory(allocator, buffer.allocation, &data);
    return data;
}

void Device::unmap(const Buffer& buffer) const {
    vmaUnmapMemory(allocator, buffer.allocation);
}

VkCommandPool Device::createCommandPool(uint32_t queue_index) const {
    VkCommandPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = queue_index;

    VkCommandPool pool = {};
    vkCreateCommandPool(device, &info, nullptr, &pool);
    return pool;
}

void Device::resetCommandPool(VkCommandPool pool) const {
    vkResetCommandPool(device, pool, 0);
}

VkCommandBuffer Device::createCommandBuffer(VkCommandPool pool) const {
    VkCommandBufferAllocateInfo info = {};
    info.commandPool = pool;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandBufferCount = 1;

    VkCommandBuffer buffer = {};
    vkAllocateCommandBuffers(device, &info, &buffer);

    return buffer;
}

VkImageView Device::createDepthStencil(const Image& image) const {
    VkImageViewCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = image.image;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = image.format;
    info.subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
                             .levelCount = 1,
                             .layerCount = 1};

    VkImageView view;
    vkCreateImageView(device, &info, nullptr, &view);
    return view;
}

VkImageView Device::createRenderTarget(const Image& image) const {
    VkImageViewCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = image.image;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = image.format;
    info.subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                             .levelCount = 1,
                             .layerCount = 1};

    VkImageView view;
    vkCreateImageView(device, &info, nullptr, &view);
    return view;
}

DescriptorLayout Device::getDescriptorLayout() const {
    return descriptor_layout;
}

DeviceProperties Device::getDeviceProperties() const { return properties; }

VkFence Device::createFence(bool is_signaled) const {
    VkFenceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    info.pNext = nullptr;
    if (is_signaled) info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkFence fence;
    vkCreateFence(device, &info, nullptr, &fence);
    return fence;
}

void Device::waitFence(VkFence fence) const {
    vkWaitForFences(device, 1, &fence, true, UINT64_MAX);
}

void Device::resetFence(VkFence fence) const {
    vkResetFences(device, 1, &fence);
}

GraphicsPipeline Device::createGraphicsPipeline(
    const VkGraphicsPipelineCreateInfo& pipeline_info) const {
    GraphicsPipeline pipeline;
    VkResult result = vkCreateGraphicsPipelines(
        device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline.pipeline);

    pipeline.layout = pipeline_info.layout;
    return pipeline;
}

VkSampler Device::createSampler(const VkSamplerCreateInfo& sampler_info) const {
    VkSampler sampler = {};
    vkCreateSampler(device, &sampler_info, nullptr, &sampler);

    return sampler;
}

Semaphore Device::createSemaphore() const {
    Semaphore semaphore;

    VkSemaphoreCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_SEMAPHORE_TYPE_BINARY;

    vkCreateSemaphore(device, &info, nullptr, &semaphore.semaphore);

    return semaphore;
}

void Device::createDescriptorLayout() {
    VkDescriptorSetLayoutBinding bindings[2] = {};
    bindings[TEXTURE_BINDING_INDEX].descriptorType =
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    bindings[TEXTURE_BINDING_INDEX].binding = TEXTURE_BINDING_INDEX;
    bindings[TEXTURE_BINDING_INDEX].stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
    bindings[TEXTURE_BINDING_INDEX].descriptorCount =
        MAX_TEXTURE_DESCRIPTORS_COUNT;

    bindings[SAMPLER_BINDING_INDEX].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    bindings[SAMPLER_BINDING_INDEX].binding = SAMPLER_BINDING_INDEX;
    bindings[SAMPLER_BINDING_INDEX].stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
    bindings[SAMPLER_BINDING_INDEX].descriptorCount =
        MAX_SAMPLE_DESCRIPTORS_COUNT;

    VkDescriptorSetLayoutCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT;
    info.bindingCount = 2;
    info.pBindings = bindings;

    vkCreateDescriptorSetLayout(device, &info, nullptr,
                                &descriptor_layout.layout);

    size_t set_size;
    vkGetDescriptorSetLayoutSizeEXT(device, descriptor_layout.layout,
                                    &descriptor_layout.layout_size);

    vkGetDescriptorSetLayoutBindingOffsetEXT(
        device, descriptor_layout.layout, TEXTURE_BINDING_INDEX,
        &descriptor_layout.texture_descriptors_offset);
    vkGetDescriptorSetLayoutBindingOffsetEXT(
        device, descriptor_layout.layout, SAMPLER_BINDING_INDEX,
        &descriptor_layout.sampler_descriptors_offset);
}

VkShaderModule Device::createShader(const uint32_t* shader_data,
                                    size_t shader_data_size) const {
    VkShaderModuleCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pCode = shader_data;
    info.codeSize = shader_data_size;

    VkShaderModule module = {};
    vkCreateShaderModule(device, &info, nullptr, &module);

    return module;
}

void Device::writeDescriptor(const VkDescriptorGetInfoEXT& info,
                             size_t descriptor_size, void* dst) const {
    vkGetDescriptorEXT(device, &info, descriptor_size, dst);
}

void Device::waitIdle() const { vkDeviceWaitIdle(device); }

VkDevice Device::getHandle() const { return device; }

}  // namespace Graphics