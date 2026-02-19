#pragma once

#include <VkBootstrap.h>

#include "Fence.h"

namespace Graphics::Internal {

struct SwapChain {
    vkb::Swapchain swap_chain;

    Fence in_flight_fence;
};

}  // namespace Graphics::Internal