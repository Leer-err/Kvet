#include "GraphicsResourceManager.h"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

#include "Graphics.h"
#include "GraphicsPipelineBuilder.h"
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
    const File::Texture& file) {
    auto texture_result =
        TextureBuilder(VK_FORMAT_R8G8B8A8_SRGB, file.width, file.height)
            .isShaderResource()
            .isCopyDestination()
            .create(device);

    if (texture_result.isError()) return std::nullopt;

    auto handle = texture_result.getResult();
    staging_buffer.stageTexture(handle, file.data.data(), file.data.size());

    if (texture_index.add(file.name, handle) == false) return std::nullopt;

    return handle;
}

std::optional<TextureHandle> ResourceManager::getTexture(
    std::string_view name) {
    return texture_index.get(name);
}

std::optional<MeshHandle> ResourceManager::addMesh(const File::Mesh& mesh) {
    auto vertex_data_ptr = std::bit_cast<uint8_t*>(mesh.vertices.data());
    auto meshlet_data_ptr = std::bit_cast<uint8_t*>(mesh.meshlets.data());
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

    if (mesh_result.isError()) return std::nullopt;

    auto handle = mesh_result.getResult();

    if (mesh_index.add(mesh.name, handle) == false) return std::nullopt;

    return handle;
}

std::optional<MeshHandle> ResourceManager::getMesh(std::string_view name) {
    return mesh_index.get(name);
}

std::optional<GraphicsPipelineHandle> ResourceManager::addPipeline(
    const File::Pipeline& pipeline) {
    auto pipeline_builder = GraphicsPipelineBuilder(pipeline.data);
    if (pipeline.depth_test_enable == false)
        pipeline_builder.disableDepthTest();
    if (pipeline.depth_write_enble == true) pipeline_builder.writesDepth();

    auto pipeline_result = pipeline_builder.create(device);
    if (pipeline_result.isError()) return std::nullopt;

    auto handle = pipeline_result.getResult();
    if (pipeline_index.add(pipeline.name, handle) == false) return std::nullopt;

    return handle;
}

std::optional<GraphicsPipelineHandle> ResourceManager::getPipeline(
    std::string_view name) {
    return pipeline_index.get(name);
}

}  // namespace Graphics