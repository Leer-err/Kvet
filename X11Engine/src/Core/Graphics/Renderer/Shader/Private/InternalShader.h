#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

struct Shader {
    Shader() noexcept {}
    ~Shader() noexcept {}

    VkPipelineShaderStageCreateInfo info;
};

}  // namespace Graphics::Internal