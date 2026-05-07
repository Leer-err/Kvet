#pragma once

#include <vulkan/vulkan.h>

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Device.h"

namespace Graphics {

class ShaderRegistry {
   public:
    ShaderRegistry(const Device& device);
    ~ShaderRegistry();

    std::optional<VkShaderModule> getModule(const std::string& name);
    std::optional<std::vector<char>> getShaderBytecode(const std::string& name);

   private:
    std::optional<VkShaderModule> loadModule(const std::string& filename);
    std::optional<std::vector<char>> readFile(const std::string& filename);

    const Device& device;

    std::unordered_map<std::string, VkShaderModule> shader_modules;
    std::unordered_map<std::string, std::vector<char>> shader_sources;
};

}  // namespace Graphics