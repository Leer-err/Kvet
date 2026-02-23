#include "SwapChain.h"

#include <vulkan/vulkan_core.h>

#include <cstdint>

#include "GraphicsResources.h"
#include "ImageBarrier.h"
#include "InternalSwapChain.h"
#include "Semaphore.h"
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
    auto device = Resources::get().getDevice();

    auto& frame = Resources::get().getFrameInFlight();
    auto& command_buffer = frame.buffer;
    command_buffer.begin();

    Internal::ImageBarrier barrier_builder(
        swap_chain->images[swap_chain->image_index],
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    barrier_builder.src_stage_mask =
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    barrier_builder.src_access_mask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;

    auto barrier = barrier_builder.create();

    VkDependencyInfo dep = {};
    dep.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dep.imageMemoryBarrierCount = 1;
    dep.pImageMemoryBarriers = &barrier;

    vkCmdPipelineBarrier2(command_buffer.buffer, &dep);

    command_buffer.end();

    VkCommandBufferSubmitInfo command_buffer_info = {};
    command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
    command_buffer_info.commandBuffer = command_buffer.buffer;
    command_buffer_info.deviceMask = 0;

    VkSemaphoreSubmitInfo semaphore_info = {};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    semaphore_info.semaphore = frame.ready_for_present.semaphore;

    VkSemaphoreSubmitInfo wait_info = {};
    wait_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    wait_info.semaphore = frame.ready_for_render.semaphore;

    VkSubmitInfo2 submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    submit_info.waitSemaphoreInfoCount = 1;
    submit_info.pWaitSemaphoreInfos = &wait_info;
    submit_info.signalSemaphoreInfoCount = 1;
    submit_info.pSignalSemaphoreInfos = &semaphore_info;
    submit_info.commandBufferInfoCount = 1;
    submit_info.pCommandBufferInfos = &command_buffer_info;

    vkQueueSubmit2(Resources::get().getGraphicsQueue(), 1, &submit_info,
                   frame.render_finished.fence);

    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.pNext = nullptr;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &frame.ready_for_present.semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = swap_chains;
    info.pImageIndices = &swap_chain->image_index;
    info.pResults = nullptr;

    auto queue = Resources::get().getPresentationQueue();
    vkQueuePresentKHR(queue, &info);
}

Texture SwapChain::getBackbuffer() {
    auto device = Resources::get().getDevice();
    auto& frame = Resources::get().getFrameInFlight();

    vkWaitForFences(device, 1, &frame.render_finished.fence, true, UINT64_MAX);
    vkResetFences(device, 1, &frame.render_finished.fence);

    auto vk_swap_chain = swap_chain->swap_chain.swapchain;
    auto image_index_ptr = &swap_chain->image_index;
    vkAcquireNextImageKHR(device, vk_swap_chain, UINT64_MAX,
                          frame.ready_for_render.semaphore, VK_NULL_HANDLE,
                          image_index_ptr);

    return Texture();
}

Internal::SwapChain* SwapChain::getInternal() const { return swap_chain.get(); }

}  // namespace Graphics