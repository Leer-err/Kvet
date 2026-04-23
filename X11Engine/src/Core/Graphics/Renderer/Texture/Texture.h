#pragma once

#include <cstdint>
#include <memory>

#include "GraphicsInternalsForward.h"

namespace Graphics {

class Texture {
   public:
    enum class Format { RGBA8, RGBA8_SRGB };

    Texture() = default;

    uint32_t getWidth() const;
    uint32_t getHeight() const;

    Texture(const Internal::Texture& texture, bool should_be_destroyed = true);
    Internal::Texture* getInternal() const;

   private:
    std::shared_ptr<Internal::WrappedTexture> texture;
};

}  // namespace Graphics