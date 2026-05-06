#pragma once

#include <VkBootstrap.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "GraphicsConfig.h"
#include "Image.h"
#include "Queue.h"
#include "Semaphore.h"

namespace Graphics {

class SwapChain {
   public:
    struct BackBuffer {
        Image backbuffer;
        Semaphore ready_for_present;
    };

    SwapChain() = default;
    SwapChain(vkb::Device device, Queue presentation_queue, uint32_t width,
              uint32_t height, Config::BufferingMode buffering_mode);

    void destroy();

    void present();

    BackBuffer getBackbuffer(const Semaphore& ready_for_render);

   private:
    static constexpr size_t SWAP_CHAIN_MAX_SIZE = 3;

    vkb::Device device;
    Queue queue;

    vkb::Swapchain swap_chain;

    Image images[SWAP_CHAIN_MAX_SIZE];
    Semaphore semaphores[SWAP_CHAIN_MAX_SIZE];
    uint32_t image_index;
    int swap_chain_size;
};

}  // namespace Graphics