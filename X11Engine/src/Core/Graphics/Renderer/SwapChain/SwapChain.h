#pragma once

#include <VkBootstrap.h>

class Texture;

class SwapChain {
    friend class SwapChainBuilder;

   public:
    SwapChain() {}

    void present();

    Texture getBackbuffer();

   protected:
    SwapChain(vkb::Swapchain swap_chain);

   private:
    vkb::Swapchain swap_chain;
};