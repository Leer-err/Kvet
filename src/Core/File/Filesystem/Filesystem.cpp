#include "Filesystem.h"

#include "Readers.h"

namespace File {

Result<TextureFile, Error> Filesystem::getTexture(std::string_view path) {
    auto full_path = get().base / path;

    return readTexture(full_path);
}

Result<MeshFile, Error> Filesystem::getMesh(std::string_view path) {
    auto full_path = get().base / path;

    return readMesh(full_path);
}

Result<EffectFile, Error> Filesystem::getEffect(std::string_view path) {
    auto full_path = get().base / path;

    return readEffect(full_path);
}

Filesystem& Filesystem::get() {
    static Filesystem instance;
    return instance;
}

}  // namespace File