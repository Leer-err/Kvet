#include "EssentialsLoader.h"

#include <filesystem>

#include "AssetManager.h"
#include "Graphics.h"
#include "LoggerFactory.h"

namespace fs = std::filesystem;

namespace Essentials {

constexpr auto TEXTURES_LIST = {""};
constexpr auto MESHES_LIST = {"Base/Sphere", "Base/SkySphere", "Base/Quad",
                              "Base/ScreenQuad"};

static bool readMesh(std::string_view path) {
    Asset::Manager::getMesh(path);

    return true;
}

bool load() {
    auto logger = LoggerFactory::getLogger("EssentialsLoader");

    auto result = true;

    for (const auto& mesh : MESHES_LIST) {
        if (readMesh(mesh) == false) {
            result = false;
            logger.error("Failed to load mesh: {}", mesh);
        }
    }

    return result;
}

}  // namespace Essentials