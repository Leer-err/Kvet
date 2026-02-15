#pragma once

#include "Result.h"
#include "Texture.h"

namespace Graphics {

enum class TextureError {
    UnsupportedFormat,
    NoDataForImmutableResource,
    WriteFromGPUAndCPU
};

class TextureBuilder {
   public:
    TextureBuilder(Texture::Format format, uint32_t width, uint32_t height);

    TextureBuilder& isShaderResource();
    TextureBuilder& isRenderTarget();
    TextureBuilder& isDepthStencil();
    TextureBuilder& isCPUWritable();
    TextureBuilder& isCopySource();
    TextureBuilder& isCopyDestination();

    Result<Texture, TextureError> create();

   private:
    const char* data;
    Texture::Format format;
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