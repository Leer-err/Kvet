#include "SwapChainBuilder.h"

#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>

#include "Fence.h"
#include "GraphicsResources.h"
#include "InternalSwapChain.h"

namespace Graphics {

SwapChainBuilder::SwapChainBuilder(uint32_t width, uint32_t height)
    : width(width), height(height), is_fullscreen(false) {}

SwapChain SwapChainBuilder::create() {
    auto device = Graphics::Resources::get().getVKBDevice();

    vkb::SwapchainBuilder swapchain_builder{device};

    auto swap_ret = swapchain_builder.build();
    if (!swap_ret) {
    }

    Internal::SwapChain swap_chain = {};
    swap_chain.swap_chain = swap_ret.value();
    swap_chain.in_flight_fence = Internal::Fence(true);

    return SwapChain(std::move(swap_chain));
}

}  // namespace Graphics