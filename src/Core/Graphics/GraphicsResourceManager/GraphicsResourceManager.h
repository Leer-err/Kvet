#pragma once

#include <optional>
#include <string_view>

#include "Handles.h"
#include "ResourceRegistry.h"

namespace Graphics {

class Texture;

using TextureIndex = ResourceIndex<Texture>;

class GraphicsResourceManager {
   public:
    void addTexture(std::string_view name, TextureHandle texture);
    std::optional<TextureHandle> getTexture(std::string_view name) const;

   private:
    TextureIndex texture_index;
};

}  // namespace Graphics