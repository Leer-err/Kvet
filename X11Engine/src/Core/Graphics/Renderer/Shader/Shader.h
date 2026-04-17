#pragma once

#include <memory>

#include "GraphicsInternalsForward.h"

namespace Graphics {

class Shader {
    friend class ShaderBuilder;
    friend class Context;

   public:
    Shader();
    ~Shader();

    Shader(Shader&&);
    Shader& operator=(Shader&&);

   protected:
    Shader(Internal::Shader&& shader);
    Internal::Shader* getInternal() const;

   private:
    std::unique_ptr<Internal::Shader> shader;
};

}  // namespace Graphics