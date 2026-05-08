#pragma once

#include <vulkan/vulkan.h>

#include <cstdint>

#include "EngineData.h"
#include "Image.h"
#include "Result.h"

namespace Graphics {

class ImageBuilder {
   public:
    ImageBuilder(const EngineData& engine_data, VkFormat format, uint32_t width,
                 uint32_t height);

    ImageBuilder& isShaderResource();
    ImageBuilder& isRenderTarget();
    ImageBuilder& isDepthStencil();
    ImageBuilder& isCopySource();
    ImageBuilder& isCopyDestination();

    Result<Image, ImageError> create();

   private:
    EngineData engine_data;

    VkImageCreateInfo image_info;
    VmaAllocationCreateInfo alloc_info;
};

}  // namespace Graphics