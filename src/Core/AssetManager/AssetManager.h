#pragma once

#include <string_view>

#include "Handles.h"
#include "Result.h"

namespace Asset {

enum class Error {};

class Manager {
   public:
    static Result<Graphics::TextureHandle, Error> getTexture(
        std::string_view name);
    static Result<Graphics::MeshHandle, Error> getMesh(std::string_view name);

   private:
    static Manager& get();
};

}  // namespace Asset