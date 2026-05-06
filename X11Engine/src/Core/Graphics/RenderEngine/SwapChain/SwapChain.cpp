#include "SwapChain.h"

#include <vulkan/vulkan.h>

#include <cstdint>

#include "Queue.h"
#include "Semaphore.h"
#include "VkBootstrap.h"

namespace Graphics {

SwapChain::SwapChain(vkb::Device device, Queue presentation_queue,
                     uint32_t width, uint32_t height,
                     Config::BufferingMode buffering_mode)
    : device(device), queue(presentation_queue) {
    vkb::SwapchainBuilder swapchain_builder{device};

    swap_chain_size = 2;
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
            swap_chain_size = 3;
            break;
    }

    VkSurfaceFormatKHR format = {};
    format.format = VK_FORMAT_R8G8B8A8_SRGB;
    format.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchain_builder.set_desired_format(format);
    swapchain_builder.add_fallback_present_mode(mode);
    swapchain_builder.set_desired_min_image_count(swap_chain_size);
    swapchain_builder.set_image_usage_flags(
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);

    auto swap_ret = swapchain_builder.build();
    if (!swap_ret) {
    }

    swap_chain = swap_ret.value();

    auto swap_cahin_images = swap_chain.get_images().value();
    swap_chain_size = swap_cahin_images.size();
    for (int i = 0; i < swap_chain_size; i++) {
        Image image = {};
        image.image = swap_cahin_images[i];
        image.format = format.format;

        images[i] = image;
        semaphores[i] = Semaphore::create();
    }
}

void SwapChain::destroy() {
    vkDestroySwapchainKHR(device, swap_chain, nullptr);

    for (int i = 0; i < swap_chain_size; i++) {
        semaphores[i].destroy();
    }
}

void SwapChain::present() {
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.pNext = nullptr;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &semaphores[image_index].semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &swap_chain.swapchain;
    info.pImageIndices = &image_index;
    info.pResults = nullptr;

    vkQueuePresentKHR(queue.queue, &info);
}

SwapChain::BackBuffer SwapChain::getBackbuffer(
    const Semaphore& ready_for_render) {
    vkAcquireNextImageKHR(device, swap_chain, UINT64_MAX,
                          ready_for_render.semaphore, VK_NULL_HANDLE,
                          &image_index);

    BackBuffer backbuffer = {};
    backbuffer.backbuffer = images[image_index];
    backbuffer.ready_for_present = semaphores[image_index];

    return backbuffer;
}

}  // namespace Graphics