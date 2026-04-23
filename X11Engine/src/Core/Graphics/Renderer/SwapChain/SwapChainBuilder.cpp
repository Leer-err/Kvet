#include "SwapChainBuilder.h"

#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>

#include "Fence.h"
#include "GraphicsConfig.h"
#include "GraphicsResources.h"
#include "InternalSwapChain.h"
#include "Semaphore.h"

namespace Graphics {

SwapChainBuilder::SwapChainBuilder(uint32_t width, uint32_t height,
                                   Config::BufferingMode buffering_mode)
    : width(width), height(height), buffering_mode(buffering_mode) {}

SwapChain SwapChainBuilder::create() {
    auto device = Graphics::Resources::get().getVKBDevice();

    vkb::SwapchainBuilder swapchain_builder{device};

    uint32_t queue_size = 2;
    VkPresentModeKHR mode;
    switch (buffering_mode) {
        case Config::BufferingMode::NoBuffering:
            mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            break;
        case Config::BufferingMode::VSyncNoBuffering:
            mode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        case Config::BufferingMode::DoubleBuffering:
            mode = VK_PRESENT_MODE_FIFO_KHR;
            break;
        case Config::BufferingMode::TripleBuffering:
            mode = VK_PRESENT_MODE_FIFO_KHR;
            queue_size = 3;
            break;
    }

    VkSurfaceFormatKHR format = {};
    format.format = VK_FORMAT_R8G8B8A8_SRGB;
    format.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchain_builder.set_desired_format(format);
    swapchain_builder.add_fallback_present_mode(mode);
    swapchain_builder.set_desired_min_image_count(queue_size);

    auto swap_ret = swapchain_builder.build();
    if (!swap_ret) {
    }

    Internal::SwapChain swap_chain = {};
    swap_chain.swap_chain = swap_ret.value();

    auto swap_cahin_images = swap_ret.value().get_images().value();
    for (int i = 0; i < queue_size; i++) {
        swap_chain.images[i] = swap_cahin_images[i];
        swap_chain.semaphores[i] = Internal::Semaphore::create();
    }

    return SwapChain(std::move(swap_chain));
}

}  // namespace Graphics