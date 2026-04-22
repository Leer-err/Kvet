#pragma once

#include <memory>

#include "GraphicsInternalsForward.h"

namespace Graphics {

class Shader {
   public:
    Shader(Internal::Shader&& shader);
    Internal::Shader* getInternal() const;

   private:
    std::shared_ptr<Internal::Shader> shader;
};

}  // namespace Graphics