#include "InputLayoutBuilder.h"

#include <vulkan/vulkan_core.h>

#include <cstddef>
#include <cstdint>
#include <vector>

#include "InputLayout.h"

namespace Graphics {

constexpr size_t getFormatSize(VkFormat format) {
    switch (format) {
        case VK_FORMAT_R32G32B32_SFLOAT:
            return sizeof(float) * 3;
        default:
            return 0;
    }

    return 0;
}

InputLayoutBuilder::InputLayoutBuilder() : elements() {}

InputLayoutBuilder::~InputLayoutBuilder() {}

InputLayoutBuilder& InputLayoutBuilder::addElement(VkFormat format) {
    elements.push_back(format);

    return *this;
}

InputLayoutBuilder& InputLayoutBuilder::setPushConstantsSize(size_t size) {
    push_constants_size = size;

    return *this;
}

InputLayout InputLayoutBuilder::create() {
    std::vector<VkVertexInputAttributeDescription> vertexAttributes;

    size_t vertex_size = 0;
    size_t element_index = 0;

    auto result = InputLayout{};
    result.elements.reserve(elements.size());

    for (const auto& format : elements) {
        auto format_size = getFormatSize(format);

        if (format_size == 0) throw;

        VkVertexInputAttributeDescription desc = {};
        desc.binding = 0;
        desc.format = format;
        desc.location = element_index;
        desc.offset = vertex_size;

        result.elements.push_back(desc);

        vertex_size += format_size;
        element_index++;
    }

    VkVertexInputBindingDescription binding;
    binding.binding = 0;
    binding.stride = static_cast<uint32_t>(vertex_size);
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    result.buffer_binding_description = binding;
    result.push_constants_size = push_constants_size;

    return result;
}

}  // namespace Graphics