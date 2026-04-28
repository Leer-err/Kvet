#pragma once

#include <vulkan/vulkan.h>

#include <vector>
namespace Graphics {

struct InputLayout {
    std::vector<VkVertexInputAttributeDescription> elements;
    VkVertexInputBindingDescription buffer_binding_description;
    size_t push_constants_size;
};

}  // namespace Graphics
