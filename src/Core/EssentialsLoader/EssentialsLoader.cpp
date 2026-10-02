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
constexpr auto PIPELINES_LIST = {"Shaders/StarPipeline"};

bool load() {
    auto logger = LoggerFactory::getLogger("EssentialsLoader");

    for (const auto& mesh : MESHES_LIST) {
        auto result = Asset::Manager::getMesh(mesh);
        if (result.isError()) {
            logger.error("Failed to load mesh: {}", mesh);
            return false;
        }
    }

    for (const auto& pipeline : PIPELINES_LIST) {
        auto result = Asset::Manager::getPipeline(pipeline);
        if (result.isError()) {
            logger.error("Failed to load pipeline: {}", pipeline);
            return false;
        }
    }

    return true;
}

}  // namespace Essentials