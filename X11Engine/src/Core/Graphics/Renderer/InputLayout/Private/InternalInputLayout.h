#pragma once

#include <vulkan/vulkan_core.h>

#include <vector>

namespace Graphics::Internal {

struct InputLayout {
    void destroy();

    std::vector<VkVertexInputAttributeDescription> elements;
    VkVertexInputBindingDescription buffer_binding_description;
};

}  // namespace Graphics::Internal