#pragma once

#include <optional>
#include <string_view>

#include "Device.h"
#include "Graphics.h"
#include "Handles.h"
#include "ResourceRegistry.h"
#include "StagingBuffer.h"

namespace Graphics {

class Texture;

using TextureIndex = ResourceIndex<Texture>;
using MeshIndex = ResourceIndex<Mesh>;
using PipelineIndex = ResourceIndex<GraphicsPipeline>;

class ResourceManager final : public IResourceManager {
   public:
    ResourceManager(Device& device, StagingBuffer& staging_buffer);

    bool addTexture(std::string_view name, TextureHandle texture);
    std::optional<TextureHandle> addTexture(const File::Texture& file) override;
    std::optional<TextureHandle> getTexture(std::string_view name) override;

    std::optional<MeshHandle> addMesh(const File::Mesh& texture) override;
    std::optional<MeshHandle> getMesh(std::string_view name) override;

   private:
    TextureIndex texture_index;
    MeshIndex mesh_index;
    PipelineIndex pipeline_index;

    Device& device;
    StagingBuffer& staging_buffer;
};

}  // namespace Graphics