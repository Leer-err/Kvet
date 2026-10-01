#pragma once

#include <vulkan/vulkan.h>

#include <string>

#include "Device.h"
#include "GraphicsPipeline.h"
#include "Result.h"
#include "Shader.h"

namespace Graphics {

class GraphicsPipelineBuilder {
   public:
    GraphicsPipelineBuilder(std::span<const uint8_t> shader_bytecode);

    GraphicsPipelineBuilder& setRasterizer(
        VkPipelineRasterizationStateCreateInfo rasterizer);

    GraphicsPipelineBuilder& setRenderTargetFormat(VkFormat format);

    GraphicsPipelineBuilder& writesDepth();
    GraphicsPipelineBuilder& disableDepthTest();

    Result<GraphicsPipelineHandle, GraphicsPipeline::Error> create(
        Device& device);

   private:
    static Result<std::vector<size_t>, GraphicsPipeline::Error>
    pushConstantsSize(std::span<const uint8_t> bytecode);

    Result<Shader, GraphicsPipeline::Error> createShader(
        Device& device, const std::string& filename,
        const std::string& entrypoint, VkShaderStageFlagBits stage);

    static VkPipelineColorBlendAttachmentState blendingSettings(
        bool alpha_blend_enable);
    static VkPipelineMultisampleStateCreateInfo multisamplingSettings();
    static VkPipelineDepthStencilStateCreateInfo depthSettings(bool test,
                                                               bool write);
    static VkPipelineRenderingCreateInfo renderingSettings(
        const DeviceProperties& properties,
        const VkFormat* render_target_format);
    static VkPipelineDynamicStateCreateInfo dynamicStateSettings();
    static VkPipelineViewportStateCreateInfo viewportSettings();
    static VkPipelineInputAssemblyStateCreateInfo inputAssemblySettings();

    std::span<const uint8_t> shader_bytecode;

    bool depth_write = false;
    bool depth_enabled = true;

    VkFormat render_target_format;
    VkPipelineRasterizationStateCreateInfo rasterization_state;
};

}  // namespace Graphics