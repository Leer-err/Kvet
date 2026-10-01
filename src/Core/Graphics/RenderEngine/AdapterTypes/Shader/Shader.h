#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <span>
#include <string>

#include "Result.h"

namespace Graphics {

enum class ShaderError {};

class Shader {
   public:
    static Result<Shader, ShaderError> create(
        VkDevice device, std::span<const uint8_t> bytecode);

    Shader(VkDevice device, VkShaderModule module);
    ~Shader();

    Shader& operator=(Shader&& other) noexcept;
    Shader(Shader&& other) noexcept;
    Shader& operator=(const Shader& other) = delete;
    Shader(const Shader& other) = delete;

    VkShaderModule getShader() const;

   private:
    VkShaderModule module;
    VkDevice device;
};

}  // namespace Graphics