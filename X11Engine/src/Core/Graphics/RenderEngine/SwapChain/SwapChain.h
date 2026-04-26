#pragma once

#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>

#include "GraphicsConfig.h"
#include "Image.h"
#include "Semaphore.h"

namespace Graphics {

class SwapChain {
   public:
    struct BackBuffer {
        Image backbuffer;
        Semaphore ready_for_present;
    };

    SwapChain() = default;
    SwapChain(uint32_t width, uint32_t height,
              Config::BufferingMode buffering_mode);

    void destroy();

    void present();

    BackBuffer getBackbuffer();

   private:
    static constexpr size_t SWAP_CHAIN_MAX_SIZE = 3;

    vkb::Swapchain swap_chain;

    Image images[SWAP_CHAIN_MAX_SIZE];
    Semaphore semaphores[SWAP_CHAIN_MAX_SIZE];
    uint32_t image_index;
    int swap_chain_size;
};

}  // namespace Graphics