#pragma once

#include <optional>
#include <string_view>

#include "Device.h"
#include "Graphics.h"
#include "Handles.h"
#include "Readers.h"
#include "ResourceRegistry.h"

namespace Graphics {

class Texture;

using TextureIndex = ResourceIndex<Texture>;

class ResourceManager final : public IResourceManager {
   public:
    bool addTexture(std::string_view name, TextureHandle texture);
    std::optional<TextureHandle> addTexture(
        const File::TextureFile& file) override;
    std::optional<TextureHandle> getTexture(std::string_view name) override;

    std::optional<MeshHandle> addMesh(const File::MeshFile& texture) override;
    std::optional<MeshHandle> getMesh(std::string_view name) override;

   private:
    TextureIndex texture_index;

    Device& device;
};

}  // namespace Graphics