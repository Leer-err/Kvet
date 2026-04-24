#pragma once

// #include "DepthStencil.h"
// #include "RenderTarget.h"
#include <cstdint>

#include "RenderTarget.h"
#include "SwapChain.h"
#include "Texture.h"

namespace Graphics {

class Renderer {
   public:
    static Renderer& get() {
        static Renderer instance;
        return instance;
    }

    void initializeResources();

    void render();

    uint32_t getWidth() const;
    uint32_t getHeight() const;

    // RenderTarget getDefaultRenderTarget() const {
    //     return default_render_target;
    // }
    // DepthStencil getDefaultDepthStencilBuffer() const {
    //     return default_depth_stencil_buffer;
    // }

   private:
    Renderer();

    Texture render_target_texture;
    RenderTarget default_render_target;
    // DepthStencil default_depth_stencil_buffer;

    SwapChain swap_chain;

    uint32_t width;
    uint32_t height;
};

}  // namespace Graphics