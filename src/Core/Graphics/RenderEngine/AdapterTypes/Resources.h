#pragma once

#include <vulkan/vulkan.h>

namespace Graphics {

namespace Rasterizer {

VkPipelineRasterizationStateCreateInfo wireframe();

VkPipelineRasterizationStateCreateInfo fill();

};  // namespace Rasterizer

struct Semaphore {
    VkSemaphoreSubmitInfo submit() const;

    VkSemaphore semaphore;
};

}  // namespace Graphics