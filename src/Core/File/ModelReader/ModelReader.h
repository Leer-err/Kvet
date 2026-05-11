#pragma once

#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cstdint>
#include <vector>

#include "VertexFormats.h"

namespace File {

class ModelReader {
   public:
    ModelReader(const std::string& filename);

    std::vector<Vertex> readVertices() const;
    std::vector<uint32_t> readIndices() const;

   private:
    Assimp::Importer importer;

    aiMesh* mesh;
};

}  // namespace File