#pragma once

#include <VkBootstrap.h>

#include "Semaphore.h"

namespace Graphics::Internal {

constexpr size_t SWAP_CHAIN_SIZE = 3;

struct SwapChain {
    vkb::Swapchain swap_chain;

    VkImage images[SWAP_CHAIN_SIZE];
    Semaphore semaphores[SWAP_CHAIN_SIZE];
    uint32_t image_index;
};

}  // namespace Graphics::Internal