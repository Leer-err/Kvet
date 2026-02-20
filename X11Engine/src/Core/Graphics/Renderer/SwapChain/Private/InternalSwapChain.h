#pragma once

#include <VkBootstrap.h>

#include "Fence.h"

namespace Graphics::Internal {

struct SwapChain {
    vkb::Swapchain swap_chain;

    Fence backbuffer_available;

    uint32_t next_frame_index;
};

}  // namespace Graphics::Internal