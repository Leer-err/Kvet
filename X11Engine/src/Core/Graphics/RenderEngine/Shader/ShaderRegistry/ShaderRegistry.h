#pragma once

#include <vulkan/vulkan_core.h>

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Graphics {

class ShaderRegistry {
   public:
    static ShaderRegistry& get() {
        static ShaderRegistry instance;
        return instance;
    }

    std::optional<VkShaderModule> getModule(const std::string& name);
    std::optional<std::vector<char>> getShaderBytecode(const std::string& name);

   private:
    ShaderRegistry();
    ~ShaderRegistry();

    std::optional<VkShaderModule> loadModule(const std::string& filename);
    std::optional<std::vector<char>> readFile(const std::string& filename);

    std::unordered_map<std::string, VkShaderModule> shader_modules;
    std::unordered_map<std::string, std::vector<char>> shader_sources;
};

}  // namespace Graphics