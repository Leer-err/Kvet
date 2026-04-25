#pragma once

#include <vulkan/vulkan_core.h>

#include <vector>
namespace Graphics {

struct InputLayout {
    std::vector<VkVertexInputAttributeDescription> elements;
    VkVertexInputBindingDescription buffer_binding_description;
};

}  // namespace Graphics
