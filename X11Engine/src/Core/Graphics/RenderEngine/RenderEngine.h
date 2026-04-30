#pragma once

#include <cstdint>
#include <memory>

namespace Graphics {

class RenderEngineImpl;

class RenderEngine {
   public:
    RenderEngine();
    ~RenderEngine();

    void render();

    uint32_t getWidth() const;
    uint32_t getHeight() const;

   private:
    std::unique_ptr<RenderEngineImpl> impl;
};

}  // namespace Graphics