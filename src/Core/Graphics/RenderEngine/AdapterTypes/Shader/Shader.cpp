#include "Shader.h"

#include <vulkan/vulkan_core.h>

namespace Graphics {

Result<Shader, ShaderError> Shader::create(VkDevice device,
                                           std::span<const uint8_t> bytecode) {
    VkShaderModuleCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pCode = std::bit_cast<uint32_t*>(bytecode.data());
    info.codeSize = bytecode.size();

    VkShaderModule module = {};
    vkCreateShaderModule(device, &info, nullptr, &module);

    return Shader(device, module);
}

Shader::Shader(VkDevice device, VkShaderModule module)
    : module(module), device(device) {}

Shader::~Shader() { vkDestroyShaderModule(device, module, nullptr); }

Shader& Shader::operator=(Shader&& other) noexcept {
    module = other.module;
    device = other.device;

    other.module = VK_NULL_HANDLE;

    return *this;
}

Shader::Shader(Shader&& other) noexcept
    : device(other.device), module(other.module) {
    other.module = VK_NULL_HANDLE;
}

VkShaderModule Shader::getShader() const { return module; }

}  // namespace Graphics