#pragma once

#include <cstdint>

#include "SwapChain.h"

namespace Graphics {

class SwapChainBuilder {
   public:
    SwapChainBuilder(uint32_t width, uint32_t height);

    SwapChain create();

   private:
    uint32_t width;
    uint32_t height;
    bool is_fullscreen;
};

}  // namespace Graphics