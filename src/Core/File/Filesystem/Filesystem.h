#pragma once

#include <filesystem>
#include <string>

#include "FileError.h"
#include "Readers.h"
#include "Result.h"

namespace File {

class Filesystem {
   public:
    explicit Filesystem(const std::filesystem::path& base);

    Result<TextureFile, Error> getTexture(std::string_view path);
    Result<MeshFile, Error> getMesh(std::string_view path);
    Result<Graphics::EffectDescription, Error> getEffect(std::string_view path);

   private:
    std::filesystem::path base;
};

}  // namespace File