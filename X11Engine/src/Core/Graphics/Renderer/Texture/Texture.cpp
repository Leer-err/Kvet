#include "Texture.h"

#include "InternalTexture.h"

namespace Graphics {

Texture::~Texture() = default;

Texture::Texture() : texture(std::make_unique<Internal::Texture>()) {}

Texture::Texture(Internal::Texture&& texture)
    : texture(std::make_unique<Internal::Texture>(texture)) {}

Internal::Texture* Texture::getInternal() const { return texture.get(); }

uint32_t Texture::getWidth() const { return texture->width; }

uint32_t Texture::getHeight() const { return texture->height; }

}  // namespace Graphics