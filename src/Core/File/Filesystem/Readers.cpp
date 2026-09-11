#include "Readers.h"

#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

#include "FileError.h"
#include "Property.h"
#include "Vector4.h"
#include "meshoptimizer.h"

namespace fs = std::filesystem;

using namespace nlohmann;

namespace File {

void from_json(const json& j, Vector3& vec) {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
}

void from_json(const json& j, Vector4& vec) {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
    j.at("w").get_to(vec.w);
}

template <typename T>
void from_json(const json& j, typename Property<T>::State& state) {
    j.at("ratio").get_to(state.ratio);
    j.at("value").get_to(state.value);
}

template <typename T>
void from_json(const json& j, Property<T>& desc) {
    auto type = j.at("type").get<std::string>();

    if (type == "constant") {
        auto value = j.at("value").get<T>();

        desc = Property<T>::constant(value);
    } else if (type == "random") {
        auto min = j.at("min").get<T>();
        auto max = j.at("max").get<T>();

        desc = Property<T>::random(min, max);
    } else if (type == "over_lifetime") {
        auto states = std::vector<typename Property<T>::State>();
        for (const auto& state_data : j.at("states")) {
            auto time = state_data.at("time").get<float>();
            auto value = state_data.at("value").get<T>();

            states.emplace_back(time, value);
        }

        desc = Property<T>::overLifetime(states);
    }
}

Result<TextureFile, Error> readTexture(const std::filesystem::path& path) {
    int width;
    int height;
    int channels;
    uint8_t* texture_data;

    if (fs::exists(path) == false) return Error::NotFound;

    texture_data =
        stbi_load(path.string().c_str(), &width, &height, &channels, 4);
    if (texture_data == nullptr || channels != 4) return Error::ParseError;

    size_t texture_size = width * height * channels;
    auto texture =
        TextureFile{path.stem(), static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height),
                    std::vector(texture_data, texture_data + texture_size)};

    stbi_image_free(texture_data);
    return texture;
}

static std::vector<Vertex> readVertices(const aiMesh* mesh) {
    if (!mesh) return {};

    std::vector<Vertex> vertices;

    size_t index_count = mesh->mNumVertices;
    vertices.reserve(index_count);

    for (int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        aiVector3D position = mesh->mVertices[i];
        vertex.position = Vector3(position.x, position.y, position.z);

        aiVector3D tex_coords = mesh->mTextureCoords[0][i];
        vertex.uv = Vector2(tex_coords.x, tex_coords.y);

        vertices.push_back(vertex);
    }

    return vertices;
}

static std::vector<uint32_t> readIndices(const aiMesh* mesh) {
    if (!mesh) return {};

    std::vector<uint32_t> indices;

    size_t index_count = mesh->mNumFaces * 3;
    indices.reserve(index_count);

    for (int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];

        for (int index = 0; index < 3; index++) {
            int vertex_index = face.mIndices[index];
            indices.push_back(vertex_index);
        }
    }

    return indices;
}

Result<MeshFile, Error> readMesh(const std::filesystem::path& path) {
    static Assimp::Importer importer;

    if (fs::exists(path) == false) return Error::NotFound;

    const aiScene* scene = importer.ReadFile(
        path, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                  aiProcess_JoinIdenticalVertices | aiProcess_MakeLeftHanded |
                  aiProcess_FlipUVs | aiProcess_OptimizeGraph |
                  aiProcess_SortByPType);

    if (scene == nullptr) return Error::ParseError;

    auto vertices = readVertices(scene->mMeshes[0]);
    auto indices = readIndices(scene->mMeshes[0]);

    const size_t MAX_VERTICES = 64;
    const size_t MAX_TRIANGLES = 96;
    const float CONE_WEIGHT = 0.25f;

    auto index_count = indices.size();
    auto vertex_count = vertices.size();

    size_t max_meshlets =
        meshopt_buildMeshletsBound(index_count, MAX_VERTICES, MAX_TRIANGLES);

    std::vector<meshopt_Meshlet> meshopt_meshlets(max_meshlets);
    std::vector<uint32_t> meshlet_vertices(index_count);
    std::vector<uint8_t> meshlet_triangles(index_count);

    size_t meshlet_count = meshopt_buildMeshlets(
        meshopt_meshlets.data(), meshlet_vertices.data(),
        meshlet_triangles.data(), std::bit_cast<uint32_t*>(indices.data()),
        index_count, std::bit_cast<float*>(vertices.data()), vertex_count,
        sizeof(Vertex), MAX_VERTICES, MAX_TRIANGLES, CONE_WEIGHT);

    const auto& last = meshopt_meshlets[meshlet_count - 1];

    meshlet_vertices.resize(last.vertex_offset + last.vertex_count);
    meshlet_triangles.resize(last.triangle_offset + last.triangle_count * 3);
    meshopt_meshlets.resize(meshlet_count);

    for (const auto& meshlet : meshopt_meshlets) {
        meshopt_optimizeMeshlet(&meshlet_vertices[meshlet.vertex_offset],
                                &meshlet_triangles[meshlet.triangle_offset],
                                meshlet.triangle_count, meshlet.vertex_count);
    }

    std::vector<Meshlet> meshlets;
    meshlets.reserve(meshlet_count);

    for (const auto& meshopt_meshlet : meshopt_meshlets) {
        Meshlet meshlet;
        meshlet.triangle_count = meshopt_meshlet.triangle_count;
        meshlet.triangle_offset = meshopt_meshlet.triangle_offset;
        meshlet.vertex_count = meshopt_meshlet.vertex_count;
        meshlet.vertex_offset = meshopt_meshlet.vertex_offset;

        meshlets.push_back(meshlet);
    }

    return MeshFile{path.stem(),      vertices,          meshlets,
                    meshlet_vertices, meshlet_triangles, meshlet_count};
}

Result<Graphics::EffectDescription, Error> readEffect(
    const std::filesystem::path& path) {
    auto file = std::ifstream(path);

    auto data = json{};
    file >> data;

    auto effect = Graphics::EffectDescription{};
    data.at("center").get_to(effect.center);
    data.at("extents").get_to(effect.extents);
    data.at("spawn_rate").get_to(effect.spawn_rate);
    data.at("color").get_to(effect.color);
    data.at("size").get_to(effect.size);
    data.at("rotation").get_to(effect.rotation);
    data.at("lifetime").get_to(effect.particle_lifetime);

    auto texture_path = data["texture"].get<std::string>();
    effect.texture = *readTexture(texture_path);

    return effect;
}

}  // namespace File