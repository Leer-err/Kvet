#include "EssentialsLoader.h"

#include <filesystem>

#include "AssetManager.h"
#include "Graphics.h"
#include "LoggerFactory.h"

namespace fs = std::filesystem;

namespace Essentials {

constexpr auto TEXTURES_LIST = {""};
constexpr auto MESHES_LIST = {"Sphere.fbx", "SkySphere.fbx", "Quad.fbx",
                              "ScreenQuad.fbx"};

static bool readMesh(std::string_view path) {
    Asset::Manager::getMesh(path);

    return true;
}

bool load() {
    auto logger = LoggerFactory::getLogger("EssentialsLoader");

    auto base = fs::path("./Assets/");

    auto result = true;

    for (const auto& texture : TEXTURES_LIST) {
        auto path = base / texture;
    }

    for (const auto& mesh : MESHES_LIST) {
        if (readMesh(mesh) == false) {
            result = false;
            logger.error("Failed to load mesh: {}", mesh);
        }
    }

    return result;
}

}  // namespace Essentials