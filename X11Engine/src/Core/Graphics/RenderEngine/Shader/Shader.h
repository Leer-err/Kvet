#pragma once
#include <vulkan/vulkan_core.h>

#include <string>
namespace Graphics {

struct Shader {
    std::string filename;
    VkShaderModule shader;
    std::string entrypoint;
    VkShaderStageFlagBits stage;
};

}  // namespace Graphics