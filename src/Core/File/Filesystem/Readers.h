#pragma once

#include <filesystem>

#include "FileError.h"
#include "Files.h"
#include "Result.h"

namespace File {

Result<TextureFile, Error> readTexture(const std::filesystem::path& path);

Result<MeshFile, Error> readMesh(const std::filesystem::path& path);

Result<EffectFile, Error> readEffect(const std::filesystem::path& path);

}  // namespace File