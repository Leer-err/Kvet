#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "FileError.h"
#include "Meshlet.h"
#include "Property.h"
#include "Quaternion.h"
#include "Result.h"
#include "Vector3.h"
#include "Vector4.h"
#include "VertexFormats.h"

namespace File {

struct Pipeline {
    std::string name;

    std::vector<uint8_t> data;

    bool depth_test_enable;
    bool depth_write_enble;
};

struct Texture {
    std::string name;

    uint32_t width;
    uint32_t height;

    std::vector<uint8_t> data;
};

struct Mesh {
    std::string name;

    std::vector<Vertex> vertices;

    std::vector<Meshlet> meshlets;
    std::vector<uint32_t> meshlet_vertices;
    std::vector<uint8_t> meshlet_triangles;

    size_t meshlet_count;
};

struct Effect {
    std::string name;

    Vector3 extents;
    float spawn_rate;
    float particle_lifetime;

    Property<Vector4> color;
    Property<float> size;
    Property<float> rotation;

    std::string texture_path;
};

struct RenderObject {
    std::string mesh_file;
    std::string albedo_file;
};

struct Scene {
    struct Effect {
        Vector3 position;
        std::string description;
    };

    struct RenderObject {
        Vector3 position;
        std::string description;
    };

    std::vector<Effect> effects;
    std::vector<RenderObject> render_objects;
};

class Filesystem {
    static constexpr auto BASE_PATH = "./Assets";

   public:
    Filesystem& operator=(const Filesystem&) = delete;
    Filesystem(const Filesystem&) = delete;
    Filesystem& operator=(Filesystem&&) = delete;
    Filesystem(Filesystem&&) = delete;

    static Result<Texture, Error> getTexture(std::string_view path);
    static Result<Mesh, Error> getMesh(std::string_view path);
    static Result<Effect, Error> getEffect(std::string_view path);
    static Result<RenderObject, Error> getRenderObject(std::string_view path);
    static Result<Scene, Error> getScene(std::string_view path);
    static Result<std::vector<uint8_t>, Error> getShader(std::string_view path);
    static Result<Pipeline, Error> getPipeline(std::string_view path);

   private:
    Filesystem();
    static Filesystem& get();

    std::filesystem::path base;
};

}  // namespace File