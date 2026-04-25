#pragma once

// #include "DepthStencil.h"
// #include "RenderTarget.h"
#include <cstdint>

// #include "RenderTarget.h"
#include "RenderTarget.h"
#include "Renderers/StarRenderer/StarRenderer.h"
#include "SwapChain.h"
// #include "Texture.h"

namespace Graphics {

class RendererImpl {
   public:
    RendererImpl();
    ~RendererImpl();

    void init();
    void render();

    uint32_t getWidth() const;
    uint32_t getHeight() const;

   private:
    void beginFrame();
    void endFrame();

    void waitRenderFinished();
    void prepareRenderTargetForRendering();

    void copyToBackbuffer(Image& render_target, Image& backbuffer);
    void prepareBackbufferForPresentation(Image& backbuffer);

    StarRenderer star_renderer;

    Image render_target_texture;
    RenderTarget render_target;
    // DepthStencil default_depth_stencil_buffer;

    SwapChain swap_chain;

    Buffer camera_data;

    uint32_t width;
    uint32_t height;
};

}  // namespace Graphics