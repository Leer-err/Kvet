#pragma once

#include <cstdint>
#include <memory>

namespace Graphics {

class RendererImpl;

class Renderer {
   public:
    Renderer();
    ~Renderer();

    void init();
    void render();

    uint32_t getWidth() const;
    uint32_t getHeight() const;

   private:
    std::unique_ptr<RendererImpl> impl;
};

}  // namespace Graphics