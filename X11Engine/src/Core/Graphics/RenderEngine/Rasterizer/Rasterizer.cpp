#pragma once

#include "Rasterizer.h"

#include <vulkan/vulkan.h>

namespace Graphics {

namespace Rasterizer {

VkPipelineRasterizationStateCreateInfo wireframe() {
    VkPipelineRasterizationStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.lineWidth = 1.0f;
    info.cullMode = VK_CULL_MODE_NONE;
    info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    info.polygonMode = VK_POLYGON_MODE_LINE;

    return info;
}

VkPipelineRasterizationStateCreateInfo fill() {
    VkPipelineRasterizationStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.cullMode = VK_CULL_MODE_BACK_BIT;
    info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    info.polygonMode = VK_POLYGON_MODE_FILL;

    return info;
}

}  // namespace Rasterizer

}  // namespace Graphics