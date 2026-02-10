#include "SwapChainBuilder.h"

#include <VkBootstrap.h>

#include "GraphicsResources.h"

SwapChainBuilder::SwapChainBuilder(uint32_t width, uint32_t height)
    : width(width), height(height), is_fullscreen(false) {}

SwapChain SwapChainBuilder::create() {
    auto device = Graphics::Resources::get().getDevice();

    vkb::SwapchainBuilder swapchain_builder{device};

    auto swap_ret = swapchain_builder.build();
    if (!swap_ret) {
    }
    vkb::Swapchain swapchain = swap_ret.value();

    return SwapChain(swapchain);
}