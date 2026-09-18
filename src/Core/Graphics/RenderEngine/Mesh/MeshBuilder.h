#pragma once

#include <cstddef>

#include "Device.h"
#include "GraphicsMesh.h"
#include "StagingBuffer.h"

namespace Graphics {

class MeshBuilder {
   public:
    MeshBuilder(std::span<const uint8_t> vertex_buffer,
                std::span<const uint8_t> meshlet_buffer,
                std::span<const uint8_t> meshlet_vertices,
                std::span<const uint8_t> meshlet_triangles);
    MeshBuilder(const uint8_t* vertex_buffer, size_t vertex_buffer_size,
                const uint8_t* meshlet_buffer, size_t meshlet_buffer_size,
                const uint8_t* meshlet_vertices, size_t meshlet_vertices_size,
                const uint8_t* meshlet_triangles,
                size_t meshlet_triangles_size);

    Mesh create(Device& device, StagingBuffer& staging_buffer);

   private:
    std::span<const uint8_t> vertex_data;
    std::span<const uint8_t> meshlet_data;
    std::span<const uint8_t> meshlet_vertices_data;
    std::span<const uint8_t> meshlet_triangles_data;
};

}  // namespace Graphics