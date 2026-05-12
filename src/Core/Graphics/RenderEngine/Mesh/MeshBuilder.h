#pragma once

#include <cstddef>

#include "EngineData.h"
#include "Mesh.h"

namespace Graphics {

class MeshBuilder {
   public:
    MeshBuilder(const void* vertex_data, size_t vertex_data_size,
                const void* index_data, size_t index_data_size);

    Mesh create(const EngineData& engine_data);

   private:
    const void* vertex_data;
    size_t vertex_data_size;
    const void* index_data;
    size_t index_data_size;
};

}  // namespace Graphics