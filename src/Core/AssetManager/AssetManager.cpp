#include "AssetManager.h"

namespace Asset {

Graphics::TextureHandle Manager::getTexture(std::string_view name) {
    auto handle_opt = Graphics::GraphicsResourceManager::getTexture(name);
    if (handle_opt.has_value()) return handle_opt.value();

    auto texture_result = Filesystem::readTexture(name);
    if (texture_result.isError()) return texture_result.getError();

    auto texture_load_result = Graphics::GraphicsResourceManager::addTexture(
        name, texture_result.getValue());
    if (texture_load_result.isError()) return texture_load_result.getError();

    return texture_load_result.getValue;
}

Manager& Manager::get() {
    static auto instance = Manager{};

    return instance;
}

}  // namespace Asset