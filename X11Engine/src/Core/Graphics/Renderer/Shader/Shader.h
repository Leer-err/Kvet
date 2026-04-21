#pragma once

#include <memory>

#include "GraphicsInternalsForward.h"

namespace Graphics {

class Shader {
   public:
    Shader();
    ~Shader();

    Shader(Shader&&);
    Shader& operator=(Shader&&);

    Shader(Internal::Shader&& shader);
    Internal::Shader* getInternal() const;

   private:
    std::unique_ptr<Internal::Shader> shader;
};

}  // namespace Graphics