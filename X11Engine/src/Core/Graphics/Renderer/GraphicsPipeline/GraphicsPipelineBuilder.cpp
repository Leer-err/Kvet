#include "GraphicsPipelineBuilder.h"

#include <vulkan/vulkan_core.h>

// #include "CommonRasterizers.h"
#include "GraphicsPipeline.h"
#include "GraphicsResources.h"
#include "InternalInputLayout.h"
#include "InternalShader.h"
#include "Pipeline.h"
#include "Shader.h"

namespace Graphics {

GraphicsPipelineBuilder::GraphicsPipelineBuilder(
    const InputLayout& input_layout, const Shader& vertex_shader,
    const Shader& pixel_shader)
    : vertex_shader(vertex_shader),
      pixel_shader(pixel_shader),
      input_layout(input_layout),
      //   rasterizer(Engine::Graphics::CommonRasterizers::fill()),
      default_render_target(true),
      has_depth_stencil(false) {}

// GraphicsPipelineBuilder& GraphicsPipelineBuilder::setRenderTarget(
//     RenderTarget render_target) {
//     this->render_target = render_target;
//     default_render_target = false;

//     return *this;
// }

// GraphicsPipelineBuilder& GraphicsPipelineBuilder::setDepthStencilBuffer(
//     DepthStencil depth_stencil_buffer) {
//     this->depth_stencil_buffer = depth_stencil_buffer;

//     return *this;
// }

// GraphicsPipelineBuilder& GraphicsPipelineBuilder::setRasterizerState(
//     Engine::Graphics::Rasterizer rasterizer) {
//     this->rasterizer = rasterizer;

//     return *this;
// }

static VkPipelineShaderStageCreateInfo getStageInfo(const Shader& shader) {
    auto shader_internal = shader.getInternal();

    VkPipelineShaderStageCreateInfo stage_info = {};
    stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage_info.stage = shader_internal->stage;
    stage_info.module = shader_internal->shader;
    stage_info.pName = shader_internal->entrypoint.c_str();

    return stage_info;
}

GraphicsPipeline GraphicsPipelineBuilder::create() {
    // if (default_render_target) render_target = RenderTarget::getDefault();

    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pushConstantRange.size = 0;

    VkPipelineLayoutCreateInfo pipelineLayoutCI = {};
    pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCI.setLayoutCount = 0;
    pipelineLayoutCI.pSetLayouts = nullptr;
    pipelineLayoutCI.pushConstantRangeCount = 0;
    pipelineLayoutCI.pPushConstantRanges = nullptr;

    VkPipelineLayout pipelineLayout;
    auto result =
        vkCreatePipelineLayout(Resources::get().getDevice(), &pipelineLayoutCI,
                               nullptr, &pipelineLayout);

    std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
    shader_stages.push_back(getStageInfo(vertex_shader));
    shader_stages.push_back(getStageInfo(pixel_shader));

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {};
    input_assembly_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    auto input_layout_internal = input_layout.getInternal();
    VkPipelineVertexInputStateCreateInfo vertex_input_state = {};
    vertex_input_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_state.vertexBindingDescriptionCount = 1;
    vertex_input_state.pVertexBindingDescriptions =
        &input_layout_internal->buffer_binding_description;
    vertex_input_state.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(input_layout_internal->elements.size());
    vertex_input_state.pVertexAttributeDescriptions =
        input_layout_internal->elements.data();

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

    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    multisampleState.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkGraphicsPipelineCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

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
    pipeline_info.layout = pipelineLayout;

    auto internal = Internal::Pipeline{};
    result = vkCreateGraphicsPipelines(Resources::get().getDevice(),
                                       VK_NULL_HANDLE, 1, &pipeline_info,
                                       nullptr, &internal.pipeline);

    return GraphicsPipeline(std::move(internal));
}

}  // namespace Graphics