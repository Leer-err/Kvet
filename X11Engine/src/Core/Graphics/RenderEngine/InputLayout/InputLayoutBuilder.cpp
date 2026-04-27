#include "InputLayoutBuilder.h"

#include <vulkan/vulkan_core.h>

#include <cstddef>
#include <cstdint>
#include <spirv_cross.hpp>
#include <vector>

#include "InputLayout.h"
#include "ShaderRegistry.h"
#include "spirv_reflect.hpp"

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

InputLayoutBuilder::InputLayoutBuilder(const std::string& vertex_shader_file)
    : vertex_shader_file(vertex_shader_file) {}

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

Result<InputLayout, InputLayoutBuilder::Error> InputLayoutBuilder::create() {
    if (vertex_shader_file.empty() == false) {
        auto shader_elements = getElementsFromShader(vertex_shader_file);
        auto shader_push_constants =
            getPushConstantsFromShader(vertex_shader_file);

        if (!shader_elements.isError() && !shader_push_constants.isError()) {
            elements = shader_elements.getResult();
            push_constants_size = shader_push_constants.getResult();
        }
    }

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

Result<std::vector<VkFormat>, InputLayoutBuilder::Error>
InputLayoutBuilder::getElementsFromShader(
    const std::string& vertex_shader_file) {
    auto shader_source =
        ShaderRegistry::get().getShaderBytecode(vertex_shader_file);
    if (!shader_source) return Error::FileNotFound;

    auto comp = spirv_cross::Compiler(
        reinterpret_cast<uint32_t*>(shader_source->data()),
        shader_source->size() / sizeof(uint32_t));

    auto res = comp.get_shader_resources();

    auto result = std::vector<VkFormat>(res.stage_inputs.size());
    for (const auto& stage_input : res.stage_inputs) {
        auto type = comp.get_type(stage_input.base_type_id);

        auto element_type = parseType(type);
        if (element_type.isError()) return element_type.getError();

        auto index =
            comp.get_decoration(stage_input.id, spv::DecorationLocation);

        result[index] = element_type.getResult();
    }

    return result;
}

Result<VkFormat, InputLayoutBuilder::Error> InputLayoutBuilder::parseType(
    const spirv_cross::SPIRType& type) {
    switch (type.basetype) {
        case spirv_cross::SPIRType::Float:
            switch (type.vecsize) {
                case 4:
                    return VK_FORMAT_R32G32B32A32_SFLOAT;
                case 3:
                    return VK_FORMAT_R32G32B32_SFLOAT;
                case 2:
                    return VK_FORMAT_R32G32_SFLOAT;
                case 1:
                    return VK_FORMAT_R32_SFLOAT;
                default:
                    return Error::UnsupportedElementSize;
            };
        case spirv_cross::SPIRType::Int:
            switch (type.vecsize) {
                case 4:
                    return VK_FORMAT_R32G32B32A32_SINT;
                case 3:
                    return VK_FORMAT_R32G32B32_SINT;
                case 2:
                    return VK_FORMAT_R32G32_SINT;
                case 1:
                    return VK_FORMAT_R32_SINT;
                default:
                    return Error::UnsupportedElementSize;
            };
        case spirv_cross::SPIRType::UInt:
            switch (type.vecsize) {
                case 4:
                    return VK_FORMAT_R32G32B32A32_UINT;
                case 3:
                    return VK_FORMAT_R32G32B32_UINT;
                case 2:
                    return VK_FORMAT_R32G32_UINT;
                case 1:
                    return VK_FORMAT_R32_UINT;
                default:
                    return Error::UnsupportedElementSize;
            };
        default:
            return Error::UnsupportedElementType;
    };
}

Result<size_t, InputLayoutBuilder::Error>
InputLayoutBuilder::getPushConstantsFromShader(
    const std::string& vertex_shader_file) {
    auto shader_source =
        ShaderRegistry::get().getShaderBytecode(vertex_shader_file);
    if (!shader_source) return Error::FileNotFound;

    auto comp = spirv_cross::Compiler(
        reinterpret_cast<uint32_t*>(shader_source->data()),
        shader_source->size() / sizeof(uint32_t));

    auto res = comp.get_shader_resources();

    size_t size = 0;

    for (const auto& push_constant : res.push_constant_buffers) {
        auto type = comp.get_type(push_constant.base_type_id);

        size += comp.get_declared_struct_size(type);
    }

    return size;
}

}  // namespace Graphics