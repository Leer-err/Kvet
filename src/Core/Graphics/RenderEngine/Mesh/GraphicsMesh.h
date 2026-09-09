#pragma once

#include "Buffer.h"
#include "Handles.h"

namespace Graphics {

struct Mesh {
    Mesh(BufferHandle vertex_buffer, BufferHandle meshlet_buffer,
         BufferHandle meshlet_vertices_buffer,
         BufferHandle meshlet_triangles_buffer, uint32_t meshlet_count)
        : vertex_buffer(vertex_buffer),
          meshlet_buffer(meshlet_buffer),
          meshlet_vertices_buffer(meshlet_vertices_buffer),
          meshlet_triangles_buffer(meshlet_triangles_buffer),
          meshlet_count(meshlet_count) {}

    BufferHandle vertex_buffer;

    BufferHandle meshlet_buffer;
    BufferHandle meshlet_vertices_buffer;
    BufferHandle meshlet_triangles_buffer;

    uint32_t meshlet_count;
};

}  // namespace Graphics