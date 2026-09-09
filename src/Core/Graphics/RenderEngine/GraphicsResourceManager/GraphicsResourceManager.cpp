#include "GraphicsResourceManager.h"

#include <optional>


namespace Graphics {

void GraphicsResourceManager::addTexture(std::string_view name,
                                         TextureHandle texture) {
    texture_index.add(name, texture);
}

std::optional<TextureHandle> GraphicsResourceManager::getTexture(
    std::string_view name) const {
    return texture_index.get(name);
}

}  // namespace Graphics