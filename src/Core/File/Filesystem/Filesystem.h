#pragma once

#include <filesystem>
#include <string>

#include "FileError.h"
#include "Readers.h"
#include "Result.h"

namespace File {

class Filesystem {
   public:
    Filesystem() = default;

    Filesystem& operator=(const Filesystem&) = delete;
    Filesystem(const Filesystem&) = delete;
    Filesystem& operator=(Filesystem&&) = delete;
    Filesystem(Filesystem&&) = delete;

    static Result<TextureFile, Error> getTexture(std::string_view path);
    static Result<MeshFile, Error> getMesh(std::string_view path);
    static Result<EffectFile, Error> getEffect(std::string_view path);

   private:
    static Filesystem& get();

    std::filesystem::path base;
};

}  // namespace File