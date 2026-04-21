#include "InputLayoutBuilder.h"

#include <vulkan/vulkan_core.h>

#include <cstddef>
#include <cstdint>
#include <vector>

#include "InputLayout.h"
#include "InternalInputLayout.h"

namespace Graphics {

constexpr size_t getFormatSize(InputElementFormat format) {
    switch (format) {
        case InputElementFormat::Vector3f:
            return sizeof(float) * 3;
    }

    return 0;
}

constexpr VkFormat getVulkanFormat(InputElementFormat format) {
    switch (format) {
        case InputElementFormat::Vector3f:
            return VK_FORMAT_R32G32B32_SFLOAT;
    }

    return VK_FORMAT_UNDEFINED;
}

InputLayoutBuilder::InputLayoutBuilder() : elements() {}

InputLayoutBuilder::~InputLayoutBuilder() {}

InputLayoutBuilder& InputLayoutBuilder::addElement(InputElementFormat format) {
    elements.push_back(format);

    return *this;
}

InputLayout InputLayoutBuilder::create() {
    std::vector<VkVertexInputAttributeDescription> vertexAttributes;

    size_t vertex_size = 0;
    size_t element_index = 0;

    auto internal = Internal::InputLayout{};
    internal.elements.reserve(elements.size());

    for (const auto& format : elements) {
        auto internal_format = getVulkanFormat(format);
        auto format_size = getFormatSize(format);

        if (internal_format == VK_FORMAT_UNDEFINED || format_size == 0) throw;

        VkVertexInputAttributeDescription desc = {};
        desc.binding = 0;
        desc.format = internal_format;
        desc.location = element_index;
        desc.offset = vertex_size;

        internal.elements.push_back(desc);

        vertex_size += format_size;
        element_index++;
    }

    VkVertexInputBindingDescription binding;
    binding.binding = 0;
    binding.stride = static_cast<uint32_t>(vertex_size);
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    internal.buffer_binding_description = binding;

    return InputLayout(std::move(internal));
}

}  // namespace Graphics