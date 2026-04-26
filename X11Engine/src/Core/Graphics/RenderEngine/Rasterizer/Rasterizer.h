#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics {

namespace Rasterizer {

VkPipelineRasterizationStateCreateInfo wireframe();

VkPipelineRasterizationStateCreateInfo fill();

};  // namespace Rasterizer

};  // namespace Graphics
