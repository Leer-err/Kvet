#include "GraphicsResourceManager.h"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

#include "Files.h"
#include "Graphics.h"
#include "MeshBuilder.h"
#include "Meshlet.h"
#include "StagingBuffer.h"
#include "TextureBuilder.h"
#include "VertexFormats.h"

namespace Graphics {

ResourceManager::ResourceManager(Device& device, StagingBuffer& staging_buffer)
    : device(device), staging_buffer(staging_buffer) {}

bool ResourceManager::addTexture(std::string_view name, TextureHandle texture) {
    return texture_index.add(name, texture);
}

std::optional<TextureHandle> ResourceManager::addTexture(
    const File::TextureFile& file) {
    auto texture_result =
        TextureBuilder(VK_FORMAT_R8G8B8A8_SRGB, file.width, file.height)
            .isShaderResource()
            .isCopyDestination()
            .create(device);

    if (texture_result.isError()) return std::nullopt;

    auto handle = texture_result.getResult();

    if (texture_index.add(file.name, handle) == false) return std::nullopt;

    return handle;
}

std::optional<TextureHandle> ResourceManager::getTexture(
    std::string_view name) {
    return texture_index.get(name);
}

std::optional<MeshHandle> ResourceManager::addMesh(const File::MeshFile& mesh) {
    auto vertex_data_ptr = std::bit_cast<uint8_t*>(mesh.vertices.data());
    auto meshlet_data_ptr = std::bit_cast<uint8_t*>(mesh.meshlets.size());
    auto meshlet_vertices_data_ptr =
        std::bit_cast<uint8_t*>(mesh.meshlet_vertices.data());
    auto meshlet_triangles_data_ptr =
        std::bit_cast<uint8_t*>(mesh.meshlet_triangles.data());

    auto mesh_result =
        MeshBuilder(vertex_data_ptr, mesh.vertices.size() * sizeof(Vertex),
                    meshlet_data_ptr, mesh.meshlets.size() * sizeof(Meshlet),
                    meshlet_vertices_data_ptr,
                    mesh.meshlet_vertices.size() * sizeof(uint32_t),
                    meshlet_triangles_data_ptr,
                    mesh.meshlet_triangles.size() * sizeof(uint8_t))
            .create(device, staging_buffer);

    // if (texture_result.isError()) return std::nullopt;

    // auto handle = texture_result.getResult();

    // if (mesh_index.add(mesh.name, handle) == false) return std::nullopt;

    return MeshHandle();
}

std::optional<MeshHandle> ResourceManager::getMesh(std::string_view name) {}
}  // namespace Graphics