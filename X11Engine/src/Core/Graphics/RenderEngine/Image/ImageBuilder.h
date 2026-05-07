#pragma once

#include <vulkan/vulkan.h>

#include <cstdint>

#include "APIData.h"
#include "Image.h"
#include "Result.h"

namespace Graphics {

enum class ImageError {
    UnsupportedFormat,
    NoDataForImmutableResource,
    WriteFromGPUAndCPU
};

class ImageBuilder {
   public:
    ImageBuilder(const APIData& api_data, VkFormat format, uint32_t width,
                 uint32_t height);

    ImageBuilder& isShaderResource();
    ImageBuilder& isRenderTarget();
    ImageBuilder& isDepthStencil();
    ImageBuilder& isCPUWritable();
    ImageBuilder& isCopySource();
    ImageBuilder& isCopyDestination();

    Result<Image, ImageError> create();

   private:
    const APIData& api_data;

    VkFormat format;
    uint32_t width;
    uint32_t height;

    bool shader_resource;
    bool render_target;
    bool depth_stencil;
    bool cpu_writable;
    bool is_copy_source;
    bool is_copy_target;
};

}  // namespace Graphics