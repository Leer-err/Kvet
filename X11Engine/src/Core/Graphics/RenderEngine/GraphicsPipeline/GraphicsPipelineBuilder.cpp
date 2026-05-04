#include "GraphicsPipelineBuilder.h"

#include <vulkan/vulkan.h>

#include <array>
#include <cstddef>

#include "GraphicsPipeline.h"
#include "GraphicsResources.h"
#include "InputLayout.h"
#include "InputLayoutBuilder.h"
#include "Rasterizer.h"
#include "Shader.h"
#include "ShaderBuilder.h"
#include "ShaderError.h"

namespace Graphics {

GraphicsPipelineBuilder::GraphicsPipelineBuilder(
    const std::string& vertex_shader_filename,
    const std::string& vertex_shader_entrypoint,
    const std::string& pixel_shader_filename,
    const std::string& pixel_shader_entrypoint,
    const DescriptorSet& descriptor_layout)
    : rasterizer(Graphics::Rasterizer::fill()),
      descriptor_layout(descriptor_layout) {
    auto vertex_shader_result =
        createShader(vertex_shader_filename, vertex_shader_entrypoint,
                     VK_SHADER_STAGE_VERTEX_BIT);
    auto pixel_shader_result =
        createShader(pixel_shader_filename, pixel_shader_entrypoint,
                     VK_SHADER_STAGE_FRAGMENT_BIT);

    if (vertex_shader_result.isError()) {
        error = vertex_shader_result.getError();
        return;
    }
    if (pixel_shader_result.isError()) {
        error = pixel_shader_result.getError();
        return;
    }

    vertex_shader = vertex_shader_result.getResult();
    pixel_shader = pixel_shader_result.getResult();
}

GraphicsPipelineBuilder::GraphicsPipelineBuilder(
    const Shader& vertex_shader, const Shader& pixel_shader,
    const DescriptorSet& descriptor_layout)
    : vertex_shader(vertex_shader),
      pixel_shader(pixel_shader),
      rasterizer(Graphics::Rasterizer::fill()),
      descriptor_layout(descriptor_layout) {}

static VkPipelineShaderStageCreateInfo getStageInfo(const Shader& shader) {
    VkPipelineShaderStageCreateInfo stage_info = {};
    stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage_info.stage = shader.stage;
    stage_info.module = shader.shader;
    stage_info.pName = shader.entrypoint.c_str();

    return stage_info;
}

Result<GraphicsPipeline, GraphicsPipelineBuilder::Error>
GraphicsPipelineBuilder::create() {
    // If error occured on previous build step return it immediately
    if (error) return *error;

    auto input_layout_result =
        InputLayoutBuilder(vertex_shader.filename).create();

    if (input_layout_result.isError()) {
        switch (input_layout_result.getError()) {
            case InputLayoutBuilder::Error::FileNotFound:
                return Error::ShaderFileNotFound;
            case InputLayoutBuilder::Error::UnsupportedElementFormat:
                return Error::VertexInputTypeNotSupported;
        }
    }

    auto input_layout = input_layout_result.getResult();

    auto pipeline = GraphicsPipeline{};

    createPipelineLayout(pipeline, input_layout);

    std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
    shader_stages.push_back(getStageInfo(vertex_shader));
    shader_stages.push_back(getStageInfo(pixel_shader));

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {};
    input_assembly_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineVertexInputStateCreateInfo vertex_input_state = {};
    vertex_input_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_state.vertexBindingDescriptionCount = 1;
    vertex_input_state.pVertexBindingDescriptions =
        &input_layout.buffer_binding_description;
    vertex_input_state.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(input_layout.elements.size());
    vertex_input_state.pVertexAttributeDescriptions =
        input_layout.elements.data();

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkDynamicState dynamicStates[2] = {VK_DYNAMIC_STATE_VIEWPORT,
                                       VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;

    VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
    depthStencilState.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilState.depthTestEnable = VK_TRUE;
    depthStencilState.depthWriteEnable = VK_TRUE;
    depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;

    VkFormat render_target_format = VK_FORMAT_R8G8B8A8_SRGB;
    VkPipelineRenderingCreateInfo renderingCI = {};
    renderingCI.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
    renderingCI.colorAttachmentCount = 1;
    renderingCI.pColorAttachmentFormats = &render_target_format;
    renderingCI.depthAttachmentFormat = VK_FORMAT_UNDEFINED;

    VkPipelineColorBlendAttachmentState blendAttachment = {};
    blendAttachment.colorWriteMask = 0xF;

    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    colorBlendState.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.logicOpEnable = VK_FALSE;
    colorBlendState.logicOp = VK_LOGIC_OP_COPY;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &blendAttachment;

    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
    rasterizationState.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.lineWidth = 1.0f;
    rasterizationState.cullMode = VK_CULL_MODE_NONE;

    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    multisampleState.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkGraphicsPipelineCreateInfo pipeline_info = {};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.pNext = &renderingCI;
    pipeline_info.stageCount = shader_stages.size();
    pipeline_info.pStages = shader_stages.data();
    pipeline_info.pVertexInputState = &vertex_input_state;
    pipeline_info.pInputAssemblyState = &input_assembly_state;
    pipeline_info.pViewportState = &viewportState;
    pipeline_info.pRasterizationState = &rasterizationState;
    pipeline_info.pMultisampleState = &multisampleState;
    pipeline_info.pDepthStencilState = &depthStencilState;
    pipeline_info.pColorBlendState = &colorBlendState;
    pipeline_info.pDynamicState = &dynamicState;
    pipeline_info.layout = pipeline.layout;

    VkResult result = vkCreateGraphicsPipelines(
        Resources::get().getDevice(), VK_NULL_HANDLE, 1, &pipeline_info,
        nullptr, &pipeline.pipeline);

    return pipeline;
}

void GraphicsPipelineBuilder::createPipelineLayout(GraphicsPipeline& pipeline,
                                                   const InputLayout& layout) {
    size_t push_constant_offset = 0;

    VkPushConstantRange constants = {};
    constants.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
    constants.size = layout.push_constants_size;
    constants.offset = 0;

    VkPipelineLayoutCreateInfo pipelineLayoutCI = {};
    pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCI.setLayoutCount = 1;
    pipelineLayoutCI.pSetLayouts = &descriptor_layout.layout;

    if (layout.push_constants_size != 0) {
        pipelineLayoutCI.pushConstantRangeCount = 1;
        pipelineLayoutCI.pPushConstantRanges = &constants;
    }

    auto result =
        vkCreatePipelineLayout(Resources::get().getDevice(), &pipelineLayoutCI,
                               nullptr, &pipeline.layout);
}

Result<Shader, GraphicsPipelineBuilder::Error>
GraphicsPipelineBuilder::createShader(const std::string& filename,
                                      const std::string& entrypoint,
                                      VkShaderStageFlagBits stage) {
    auto shader_build_result =
        ShaderBuilder(filename, entrypoint, stage).create();

    if (shader_build_result.isOk()) return shader_build_result.getResult();

    auto error = shader_build_result.getError();
    switch (error) {
        case ShaderError::NotFound:
            return Error::ShaderFileNotFound;
    }
}
}  // namespace Graphics