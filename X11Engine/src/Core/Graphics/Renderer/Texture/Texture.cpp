#include "Texture.h"

#include <memory>

#include "GraphicsInternalsForward.h"
#include "InternalTexture.h"
#include "ResourceWrapper.h"

namespace Graphics {

Texture::Texture(const Internal::Texture& texture, bool should_be_destroyed)
    : texture(std::make_shared<Internal::WrappedTexture>(
          texture, should_be_destroyed)) {}

Internal::Texture* Texture::getInternal() const {
    if (!texture) return nullptr;

    return texture.get()->getPtr();
}

uint32_t Texture::getWidth() const { return texture->getRef().width; }

uint32_t Texture::getHeight() const { return texture->getRef().height; }

}  // namespace Graphics