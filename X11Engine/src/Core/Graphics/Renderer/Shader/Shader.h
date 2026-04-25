#pragma once
#include <vulkan/vulkan_core.h>

#include <string>
namespace Graphics {

struct Shader {
    VkShaderModule shader;
    std::string entrypoint;
    VkShaderStageFlagBits stage;
    size_t constant_range_size;
};

}  // namespace Graphics