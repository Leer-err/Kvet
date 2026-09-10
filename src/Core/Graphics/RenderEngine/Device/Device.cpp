#include "Device.h"

#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.h>

#include <array>
#include <vector>

#include "Buffer.h"
#include "BufferBuilder.h"
#include "CommandBuffer.h"
#include "DescriptorLayout.h"
#include "DeviceProperties.h"
#include "EngineConstants.h"
#include "ExtensionFunctions.h"
#include "GraphicsPipeline.h"
#include "Handles.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "Texture.h"
#include "VkBootstrap.h"

constexpr auto TEXTURE_BINDING_INDEX = 0;
constexpr auto SAMPLER_BINDING_INDEX = 1;

constexpr auto MAX_TEXTURE_DESCRIPTORS_COUNT = 1000;
constexpr auto MAX_SAMPLER_DESCRIPTORS_COUNT = 1000;
constexpr auto MAX_BUFFERS_COUNT = 1000;

namespace Graphics {

Device::Device(const vkb::Instance& instance, const vkb::Device& device,
               VmaAllocator allocator)
    : instance(instance),
      device(device),
      allocator(allocator),
      buffer_allocator(MAX_BUFFERS_COUNT, sizeof(Buffer), alignof(Buffer)),
      buffer_registry(buffer_allocator),
      texture_allocator(MAX_TEXTURE_DESCRIPTORS_COUNT, sizeof(Texture),
                        alignof(Texture)),
      texture_registry(texture_allocator),
      descriptor_layout(createDescriptorLayout(device)),
      descriptors(createDescriptorBuffer(
          descriptor_layout.layout_size,
          properties.descriptor_buffer_properties.alignment)),
      texture_descriptor_allocator(MAX_TEXTURE_DESCRIPTORS_COUNT),
      sampler_descriptor_allocator(MAX_SAMPLER_DESCRIPTORS_COUNT),
      logger(LoggerFactory::getLogger("GraphicsDevice")) {
    properties = DeviceProperties::readProperties(device.physical_device);
}

Device::~Device() {
    vmaDestroyAllocator(allocator);
    vkb::destroy_device(device);
    vkb::destroy_instance(instance);
}

vkb::Swapchain Device::createSwapChain(VkSurfaceFormatKHR format,
                                       VkPresentModeKHR present_mode,
                                       size_t image_count,
                                       VkImageUsageFlags flags) {
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

Result<TextureHandle, TextureError> Device::createTexture(
    const VkImageCreateInfo& image_info,
    const VmaAllocationCreateInfo& alloc_info) {
    auto texture_result =
        Texture::create(device, allocator, image_info, alloc_info,
                        texture_descriptor_allocator);
    auto handle = texture_registry.create(texture_result.getResult());

    writeTextureDescriptor(handle->getDescriptor(), handle->getView());

    return handle;
}

Result<BufferHandle, BufferError> Device::createBuffer(
    const VkBufferCreateInfo& buffer_info,
    const VmaAllocationCreateInfo& alloc_info, bool is_chained) {
    auto buffer_result =
        Buffer::create(device, allocator, buffer_info, alloc_info, is_chained);
    if (buffer_result.isError()) return buffer_result.getError();

    auto handle = buffer_registry.create(buffer_result.getResult());

    return handle;
}

VkCommandPool Device::createCommandPool(uint32_t queue_index) {
    VkCommandPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = queue_index;

    VkCommandPool pool = {};
    VkResult result = vkCreateCommandPool(device, &info, nullptr, &pool);

    if (result != VK_SUCCESS)
        logger.error("Command pool creation failed with with {}",
                     string_VkResult(result));
    return pool;
}

void Device::resetCommandPool(VkCommandPool pool) const {
    vkResetCommandPool(device, pool, 0);
}

VkCommandBuffer Device::createCommandBuffer(VkCommandPool pool) {
    VkCommandBufferAllocateInfo info = {};
    info.commandPool = pool;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandBufferCount = 1;

    VkCommandBuffer buffer = {};
    VkResult result = vkAllocateCommandBuffers(device, &info, &buffer);

    if (result != VK_SUCCESS)
        logger.error("Command buffer creation failed with with {}",
                     string_VkResult(result));

    return buffer;
}

DescriptorLayout Device::getDescriptorLayout() const {
    return descriptor_layout;
}

DeviceProperties Device::getDeviceProperties() const { return properties; }

VkFence Device::createFence(bool is_signaled) {
    VkFenceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    info.pNext = nullptr;
    if (is_signaled) info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkFence fence;
    vkCreateFence(device, &info, nullptr, &fence);
    return fence;
}

void Device::waitFence(VkFence fence) {
    VkResult result = vkWaitForFences(device, 1, &fence, true, UINT64_MAX);
    if (result != VK_SUCCESS)
        logger.error("Failure waiting for fence with {}",
                     string_VkResult(result));
}

void Device::resetFence(VkFence fence) const {
    vkResetFences(device, 1, &fence);
}

VkPipelineLayout Device::createPipelineLayout(
    const std::vector<size_t>& push_constant_ranges) {
    size_t offset = 0;

    auto push_constants = std::vector<VkPushConstantRange>();
    push_constants.reserve(push_constant_ranges.size());

    for (const auto& range : push_constant_ranges) {
        VkPushConstantRange constant = {};
        constant.stageFlags = VK_SHADER_STAGE_TASK_BIT_EXT |
                              VK_SHADER_STAGE_MESH_BIT_EXT |
                              VK_SHADER_STAGE_FRAGMENT_BIT;
        constant.size = range;
        constant.offset = offset;

        offset += range;

        push_constants.push_back(constant);
    }

    VkPipelineLayoutCreateInfo pipelineLayoutCI = {};
    pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCI.setLayoutCount = 1;
    pipelineLayoutCI.pSetLayouts = &descriptor_layout.layout;

    pipelineLayoutCI.pushConstantRangeCount = push_constants.size();
    pipelineLayoutCI.pPushConstantRanges = push_constants.data();

    VkPipelineLayout pipeline_layout = {};
    auto result = vkCreatePipelineLayout(device, &pipelineLayoutCI, nullptr,
                                         &pipeline_layout);

    return pipeline_layout;
}

GraphicsPipeline Device::createGraphicsPipeline(
    const VkGraphicsPipelineCreateInfo& pipeline_info) {
    VkPipeline pipeline;
    VkResult result = vkCreateGraphicsPipelines(
        device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline);

    return GraphicsPipeline{pipeline, pipeline_info.layout, descriptors};
}

VkSampler Device::createSampler(const VkSamplerCreateInfo& sampler_info) {
    VkSampler sampler = {};
    vkCreateSampler(device, &sampler_info, nullptr, &sampler);

    return sampler;
}

Semaphore Device::createSemaphore() {
    Semaphore semaphore;

    VkSemaphoreCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_SEMAPHORE_TYPE_BINARY;

    vkCreateSemaphore(device, &info, nullptr, &semaphore.semaphore);

    return semaphore;
}

DescriptorLayout Device::createDescriptorLayout(VkDevice device) {
    DescriptorLayout layout;

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
        MAX_SAMPLER_DESCRIPTORS_COUNT;

    VkDescriptorSetLayoutCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT;
    info.bindingCount = 2;
    info.pBindings = bindings;

    vkCreateDescriptorSetLayout(device, &info, nullptr, &layout.layout);

    vkGetDescriptorSetLayoutSizeEXT(device, layout.layout, &layout.layout_size);

    vkGetDescriptorSetLayoutBindingOffsetEXT(
        device, layout.layout, TEXTURE_BINDING_INDEX,
        &layout.texture_descriptors_offset);
    vkGetDescriptorSetLayoutBindingOffsetEXT(
        device, layout.layout, SAMPLER_BINDING_INDEX,
        &layout.sampler_descriptors_offset);

    return layout;
}

VkShaderModule Device::createShader(const uint32_t* shader_data,
                                    size_t shader_data_size) {
    VkShaderModuleCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pCode = shader_data;
    info.codeSize = shader_data_size;

    VkShaderModule module = {};
    vkCreateShaderModule(device, &info, nullptr, &module);

    return module;
}

void Device::waitIdle() const { vkDeviceWaitIdle(device); }

VkInstance Device::getInstance() const { return instance; }

VkDevice Device::getDevice() const { return device; }

VkPhysicalDevice Device::getPhysicalDevice() const {
    return device.physical_device;
}

BufferRegistry& Device::getBufferRegistry() { return buffer_registry; }

TracyVkCtx Device::createTracingContext(
    const Queue& queue, const CommandBuffer& command_buffer) const {
    return TracyVkContextCalibrated(
        device.physical_device, device, queue.queue, command_buffer.buffer,
        vkGetPhysicalDeviceCalibrateableTimeDomainsEXT,
        vkGetCalibratedTimestampsEXT);
}

void Device::writeTextureDescriptor(TextureDescriptor index,
                                    VkImageView descriptor) {
    auto descriptors_ptr = descriptors->getHostAddress() +
                           descriptor_layout.texture_descriptors_count;
    auto descriptor_ptr =
        descriptors_ptr +
        properties.descriptor_buffer_properties.texture_size * index;

    VkDescriptorImageInfo image_descriptor_info = {};
    image_descriptor_info.imageView = descriptor;
    image_descriptor_info.imageLayout =
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkDescriptorGetInfoEXT info{};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT;
    info.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    info.data.pSampledImage = &image_descriptor_info;
    vkGetDescriptorEXT(device, &info,
                       properties.descriptor_buffer_properties.texture_size,
                       descriptor_ptr);
}

void Device::writeSamplerDescriptor(SamplerDescriptor index,
                                    VkSampler descriptor) {
    auto descriptors_ptr = descriptors->getHostAddress() +
                           descriptor_layout.sampler_descriptors_offset;
    auto descriptor_ptr =
        descriptors_ptr +
        properties.descriptor_buffer_properties.sampler_size * index;

    VkDescriptorGetInfoEXT info{};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT;
    info.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    info.data.pSampler = &descriptor;
    vkGetDescriptorEXT(device, &info,
                       properties.descriptor_buffer_properties.texture_size,
                       descriptor_ptr);
}

BufferHandle Device::createDescriptorBuffer(size_t set_size, size_t alignment) {
    auto aligned_size = (set_size + alignment - 1) & ~(alignment - 1);

    return BufferBuilder(aligned_size)
        .isDescriptorBuffer()
        .isCPUWritable(true, true)
        .isChained()
        .create(*this)
        .getResult();
}

}  // namespace Graphics