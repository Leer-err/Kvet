#pragma once

#include <string_view>

namespace Asset {

enum class Error {};

class Manager {
   public:
    Result<Graphics::TextureHandle, Error> getTexture(std::string_view name);

   private:
    Manager& get();
};

}  // namespace Asset