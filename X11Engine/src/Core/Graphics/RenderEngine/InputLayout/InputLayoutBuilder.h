#pragma once

#include <vulkan/vulkan.h>

#include <optional>
#include <spirv_cross.hpp>
#include <vector>

#include "InputLayout.h"
#include "Result.h"

namespace Graphics {

class InputLayoutBuilder {
   public:
    enum class Error {
        FileNotFound,
        UnsupportedElementType,
        UnsupportedElementSize
    };

    InputLayoutBuilder(const std::string& vertex_shader_file);

    InputLayoutBuilder();
    ~InputLayoutBuilder();

    InputLayoutBuilder& addElement(VkFormat format);
    InputLayoutBuilder& setPushConstantsSize(size_t size);

    Result<InputLayout, Error> create();

   private:
    Result<std::vector<VkFormat>, Error> getElementsFromShader(
        const std::string& vertex_shader_file);
    Result<size_t, Error> getPushConstantsFromShader(
        const std::string& vertex_shader_file);

    Result<VkFormat, Error> parseType(const spirv_cross::SPIRType& type);

    std::string vertex_shader_file;

    std::vector<VkFormat> elements;

    size_t push_constants_size;
};

}  // namespace Graphics