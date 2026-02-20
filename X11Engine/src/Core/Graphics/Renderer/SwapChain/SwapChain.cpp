#include "SwapChain.h"

#include <vulkan/vulkan_core.h>

#include <cstdint>

#include "GraphicsResources.h"
#include "InternalSwapChain.h"
#include "Texture.h"

namespace Graphics {

SwapChain::~SwapChain() = default;

SwapChain::SwapChain(SwapChain&&) = default;

SwapChain& SwapChain::operator=(SwapChain&&) = default;

SwapChain::SwapChain() : swap_chain(std::make_unique<Internal::SwapChain>()) {}

SwapChain::SwapChain(Internal::SwapChain&& swap_chain)
    : swap_chain(std::make_unique<Internal::SwapChain>(std::move(swap_chain))) {
}

void SwapChain::present() {
    VkSwapchainKHR swap_chains[] = {swap_chain->swap_chain};
    uint32_t image_index = swap_chain->next_frame_index;

    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.pNext = nullptr;
    info.waitSemaphoreCount = 0;
    info.pWaitSemaphores = nullptr;
    info.swapchainCount = 1;
    info.pSwapchains = swap_chains;
    info.pImageIndices = &image_index;
    info.pResults = nullptr;

    auto queue = Resources::get().getPresentationQueue();
    vkQueuePresentKHR(queue, &info);
}

Texture SwapChain::getBackbuffer() {
    auto device = Resources::get().getDevice();
    auto fence = swap_chain->backbuffer_available.fence;

    vkResetFences(device, 1, &fence);

    auto vk_swap_chain = swap_chain->swap_chain.swapchain;
    auto image_index_ptr = &swap_chain->next_frame_index;
    vkAcquireNextImageKHR(device, vk_swap_chain, UINT64_MAX, VK_NULL_HANDLE,
                          fence, image_index_ptr);

    vkWaitForFences(device, 1, &fence, true, UINT64_MAX);

    return Texture();
}

Internal::SwapChain* SwapChain::getInternal() const { return swap_chain.get(); }

}  // namespace Graphics