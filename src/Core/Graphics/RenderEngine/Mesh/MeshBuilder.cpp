#include "MeshBuilder.h"

#include "BufferBuilder.h"
#include "Device.h"
#include "GraphicsMesh.h"
#include "Meshlet.h"

namespace Graphics {

MeshBuilder::MeshBuilder(std::span<const uint8_t> vertex_buffer,
                         std::span<const uint8_t> meshlet_buffer,
                         std::span<const uint8_t> meshlet_vertices,
                         std::span<const uint8_t> meshlet_triangles)
    : vertex_data(vertex_buffer),
      meshlet_data(meshlet_buffer),
      meshlet_vertices_data(meshlet_vertices),
      meshlet_triangles_data(meshlet_triangles) {}

MeshBuilder::MeshBuilder(
    const uint8_t* vertex_buffer, size_t vertex_buffer_size,
    const uint8_t* meshlet_buffer, size_t meshlet_buffer_size,
    const uint8_t* meshlet_vertices, size_t meshlet_vertices_size,
    const uint8_t* meshlet_triangles, size_t meshlet_triangles_size)
    : vertex_data(vertex_buffer, vertex_buffer_size),
      meshlet_data(meshlet_buffer, meshlet_buffer_size),
      meshlet_vertices_data(meshlet_vertices, meshlet_vertices_size),
      meshlet_triangles_data(meshlet_triangles, meshlet_triangles_size) {}

Graphics::Mesh MeshBuilder::create(Device& device,
                                   StagingBuffer& staging_buffer) {
    auto vertex_buffer = BufferBuilder(vertex_data.size())
                             .isShaderResource()
                             .isCopyDestination()
                             .create(device)
                             .getResult();
    staging_buffer.stageBuffer(vertex_buffer, vertex_data);

    auto meshlet_buffer = BufferBuilder(meshlet_data.size())
                              .isShaderResource()
                              .isCopyDestination()
                              .create(device)
                              .getResult();
    staging_buffer.stageBuffer(meshlet_buffer, meshlet_data);

    auto meshlet_vertices_buffer = BufferBuilder(meshlet_vertices_data.size())
                                       .isShaderResource()
                                       .isCopyDestination()
                                       .create(device)
                                       .getResult();
    staging_buffer.stageBuffer(meshlet_vertices_buffer, meshlet_vertices_data);

    auto meshlet_triangles_buffer = BufferBuilder(meshlet_triangles_data.size())
                                        .isShaderResource()
                                        .isCopyDestination()
                                        .create(device)
                                        .getResult();
    staging_buffer.stageBuffer(meshlet_triangles_buffer,
                               meshlet_triangles_data);

    return Mesh(vertex_buffer, meshlet_buffer, meshlet_vertices_buffer,
                meshlet_triangles_buffer,
                meshlet_data.size() / sizeof(Meshlet));
}

}  // namespace Graphics
