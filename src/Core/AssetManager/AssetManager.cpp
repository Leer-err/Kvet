#include "AssetManager.h"

#include "Filesystem.h"
#include "Graphics.h"

namespace Asset {

Result<Graphics::TextureHandle, Error> Manager::getTexture(
    std::string_view name) {
    auto resource_manager = Graphics::getRenderEngine()->getResourceManager();
    auto handle_opt = resource_manager->getTexture(name);
    if (handle_opt.has_value()) return handle_opt.value();

    auto texture_result = File::Filesystem::getTexture(name);
    // if (texture_result.isError()) return texture_result.getError();

    auto texture_load_result =
        resource_manager->addTexture(texture_result.getResult());
    // if (texture_load_result.isError()) return texture_load_result.getError();

    return texture_load_result.value();
}

Result<Graphics::MeshHandle, Error> Manager::getMesh(std::string_view name) {
    auto resource_manager = Graphics::getRenderEngine()->getResourceManager();
    auto handle_opt = resource_manager->getMesh(name);
    if (handle_opt.has_value()) return handle_opt.value();

    auto mesh_result = File::Filesystem::getMesh(name);
    // if (texture_result.isError()) return texture_result.getError();

    auto texture_load_result =
        resource_manager->addMesh(mesh_result.getResult());
    // if (texture_load_result.isError()) return texture_load_result.getError();

    return texture_load_result.value();
}

Manager& Manager::get() {
    static auto instance = Manager{};

    return instance;
}

}  // namespace Asset