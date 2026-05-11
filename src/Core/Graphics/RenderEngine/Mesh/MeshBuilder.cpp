#include "MeshBuilder.h"

#include <cstring>

#include "BufferBuilder.h"
#include "EngineData.h"
#include "Mesh.h"

namespace Graphics {

MeshBuilder::MeshBuilder(const EngineData& engine_data, const void* vertex_data,
                         size_t vertex_data_size, const void* index_data,
                         size_t index_data_size)
    : engine_data(engine_data),
      vertex_data(vertex_data),
      vertex_data_size(vertex_data_size),
      index_data(index_data),
      index_data_size(index_data_size) {}

Mesh MeshBuilder::create() {
    Mesh mesh = {};
    mesh.vertex_buffer = BufferBuilder(engine_data, vertex_data_size)
                             .isVertexBuffer()
                             .isCPUWritable()
                             .create()
                             .getResult();
    mesh.index_buffer = BufferBuilder(engine_data, index_data_size)
                            .isIndexBuffer()
                            .isCPUWritable()
                            .create()
                            .getResult();

    auto index_buffer_ptr = engine_data.device.map(mesh.index_buffer);
    memcpy(index_buffer_ptr, index_data, index_data_size);
    engine_data.device.unmap(mesh.index_buffer);

    auto vertex_buffer_ptr = engine_data.device.map(mesh.vertex_buffer);
    memcpy(vertex_buffer_ptr, vertex_data, vertex_data_size);
    engine_data.device.unmap(mesh.vertex_buffer);

    return mesh;
}

}  // namespace Graphics
