#pragma once

#include <VkBootstrap.h>

namespace Graphics::Internal {

struct SwapChain {
    vkb::Swapchain swap_chain;

    VkImage images[3];
    uint32_t image_index;
};

}  // namespace Graphics::Internal