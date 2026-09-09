#pragma once

#include <vulkan/vulkan.h>

#include <string_view>

#include "Device.h"
#include "Handles.h"
namespace Graphics {

class TextureBuilder {
   public:
    TextureBuilder(VkFormat format, uint32_t width, uint32_t height);

    TextureBuilder& setName(std::string_view name);
    TextureBuilder& isShaderResource();
    TextureBuilder& isRenderTarget();
    TextureBuilder& isDepthStencil();
    TextureBuilder& isCopySource();
    TextureBuilder& isCopyDestination();

    Result<TextureHandle, TextureError> create(Device& device);

   private:
    VkImageCreateInfo image_info;
    VmaAllocationCreateInfo alloc_info;

    std::string name;
};

}  // namespace Graphics