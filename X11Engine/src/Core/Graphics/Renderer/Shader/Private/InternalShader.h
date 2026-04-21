#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

#include <string>

namespace Graphics::Internal {

struct Shader {
    VkShaderModule shader;
    std::string entrypoint;
    VkShaderStageFlagBits stage;
};

}  // namespace Graphics::Internal