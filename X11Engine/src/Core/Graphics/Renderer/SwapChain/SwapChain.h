#pragma once

#include <memory>

#include "GraphicsInternalsForward.h"

namespace Graphics {

class Texture;

class SwapChain {
   public:
    SwapChain();
    ~SwapChain();

    SwapChain(SwapChain&&);
    SwapChain& operator=(SwapChain&&);

    void present();

    Texture getBackbuffer();
    SwapChain(Internal::SwapChain&& swap_chain);
    Internal::SwapChain* getInternal() const;

   private:
    std::unique_ptr<Internal::SwapChain> swap_chain;
};

}  // namespace Graphics