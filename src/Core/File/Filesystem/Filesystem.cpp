#include "Filesystem.h"

#include "Readers.h"

namespace File {

Filesystem::Filesystem(const std::filesystem::path& base) : base(base) {}

Result<TextureFile, Error> Filesystem::getTexture(std::string_view path) {
    auto full_path = base / path;

    return readTexture(full_path);
}

Result<MeshFile, Error> Filesystem::getMesh(std::string_view path) {
    auto full_path = base / path;

    return readMesh(full_path);
}

Result<EffectFile, Error> Filesystem::getEffect(std::string_view path) {
    auto full_path = base / path;

    return readEffect(full_path);
}

}  // namespace File