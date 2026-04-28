#pragma once

#include <vulkan/vulkan.h>

namespace Graphics {

namespace Rasterizer {

VkPipelineRasterizationStateCreateInfo wireframe();

VkPipelineRasterizationStateCreateInfo fill();

};  // namespace Rasterizer

};  // namespace Graphics
