#pragma once

#include <cstdint>
#include <memory>

#include "GraphicsInternalsForward.h"

namespace Graphics {

class Texture {
    friend class TextureBuilder;
    friend class SwapChain;
    friend class RenderTarget;
    friend class ShaderResource;
    friend class Context;

   public:
    enum class Format { RGBA8, RGBA8_SRGB };

    Texture();
    ~Texture();

    Texture(Texture&&);
    Texture& operator=(Texture&&);

    uint32_t getWidth() const;
    uint32_t getHeight() const;

    Texture(Internal::Texture&& texture);

    Internal::Texture* getInternal() const;

   private:
    std::unique_ptr<Internal::Texture> texture;
};

}  // namespace Graphics