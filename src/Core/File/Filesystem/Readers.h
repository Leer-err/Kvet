#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "FileError.h"
#include "Meshlet.h"
#include "Result.h"
#include "VertexFormats.h"

namespace File {

struct TextureFile {
    std::string name;

    uint32_t width;
    uint32_t height;

    std::vector<uint8_t> data;
};

struct MeshFile {
    std::string name;

    std::vector<Vertex> vertices;

    std::vector<Meshlet> meshlets;
    std::vector<uint32_t> meshlet_vertices;
    std::vector<uint8_t> meshlet_triangles;

    size_t meshlet_count;
};

Result<TextureFile, Error> readTexture(const std::filesystem::path& path);

Result<MeshFile, Error> readMesh(const std::filesystem::path& path);

Result<Graphics::EffectDescription, Error> readEffect(
    const std::filesystem::path& path);

}  // namespace File