#include "ShaderRegistry.h"

#include <vulkan/vulkan_core.h>

#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include "GraphicsResources.h"

namespace Graphics {

ShaderRegistry::ShaderRegistry() {}
ShaderRegistry::~ShaderRegistry() {
    for (auto& [filename, module] : shader_modules) {
        vkDestroyShaderModule(Resources::get().getDevice(), module, nullptr);
    }
}

std::optional<VkShaderModule> ShaderRegistry::getModule(
    const std::string& filename) {
    auto it = shader_modules.find(filename);

    if (it == shader_modules.end()) return loadModule(filename);

    return it->second;
}

std::optional<std::vector<char>> ShaderRegistry::getShaderBytecode(
    const std::string& filename) {
    auto it = shader_sources.find(filename);

    if (it == shader_sources.end()) {
        if (!loadModule(filename)) return {};

        it = shader_sources.find(filename);
    }

    return it->second;
}

std::optional<VkShaderModule> ShaderRegistry::loadModule(
    const std::string& filename) {
    auto shader_bytecode = readFile(filename);
    if (shader_bytecode == std::nullopt) return std::nullopt;

    VkShaderModuleCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pCode = reinterpret_cast<const uint32_t*>(shader_bytecode->data());
    info.codeSize = shader_bytecode->size();

    VkShaderModule module = {};
    if (vkCreateShaderModule(Resources::get().getDevice(), &info, nullptr,
                             &module) != VK_SUCCESS)
        return std::nullopt;

    shader_modules.emplace(filename, module);
    shader_sources.emplace(filename, std::move(*shader_bytecode));
    return module;
}

std::optional<std::vector<char>> ShaderRegistry::readFile(
    const std::string& filename) {
    auto file = std::ifstream(filename, std::ios::binary | std::ios::ate);

    if (file.is_open() == false) return std::nullopt;

    auto file_size = file.tellg();
    auto file_buffer = std::vector<char>(file_size);

    file.seekg(0);
    file.read(file_buffer.data(), file_size);
    file.close();

    return file_buffer;
}

}  // namespace Graphics