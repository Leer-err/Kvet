#include "RenderEngineImpl.h"

#include <vulkan/vulkan.h>

#include <cstring>
#include <tracy/Tracy.hpp>

// #include "AppConfig.h"
// #include "Context.h"
// #include "Format.h"
#include "AppConfig.h"
// #include "Context.h"
#include "CommandBuffer.h"
#include "DescriptorSet/DescriptorSet.h"
#include "Fence.h"
#include "GraphicsResources.h"
// #include "RenderEnviroment.h"
#include "ImageBuilder.h"
#include "RenderPass.h"
#include "RenderTarget.h"
#include "SwapChain.h"

namespace Graphics {

RenderEngineImpl::RenderEngineImpl()
    : descriptor_set(DescriptorSet::create()), render_pass(descriptor_set) {
    frame_in_flight_index = 0;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        auto& frame_in_flight = frames_in_flight[i];
        frame_in_flight.pool = CommandPool::create();
        frame_in_flight.backbuffer_ready_for_rendering = Semaphore::create();
        frame_in_flight.finished_processing = Fence::create(true);
    }

    reinitWindowDependentResources();
}

RenderEngineImpl::~RenderEngineImpl() { swap_chain.destroy(); }

void RenderEngineImpl::render() {
    ZoneScoped;

    waitRenderFinished();

    auto pool = frames_in_flight[frame_in_flight_index].pool;
    auto cmd = pool.getCommandBuffer();

    beginFrame(cmd);

    render_pass.render(cmd, render_enviroment);

    endFrame(cmd);

    frame_in_flight_index = (frame_in_flight_index + 1) % MAX_FRAMES_IN_FLIGHT;
}

void RenderEngineImpl::beginFrame(const CommandBuffer& cmd) {
    ZoneScoped;

    cmd.begin();
    prepareRenderTargetForRendering(cmd);
}

void RenderEngineImpl::endFrame(const CommandBuffer& cmd) {
    ZoneScoped;

    auto frame_in_flight = frames_in_flight[frame_in_flight_index];

    auto [backbuffer, ready_for_present] = swap_chain.getBackbuffer(
        frame_in_flight.backbuffer_ready_for_rendering);

    copyToBackbuffer(cmd, render_target_texture, backbuffer);
    prepareBackbufferForPresentation(cmd, backbuffer);

    cmd.end();

    auto command_buffer_info = cmd.submit();
    auto signal = ready_for_present.submit();
    auto wait = frame_in_flight.backbuffer_ready_for_rendering.submit();

    VkSubmitInfo2 submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    submit_info.waitSemaphoreInfoCount = 1;
    submit_info.pWaitSemaphoreInfos = &wait;
    submit_info.signalSemaphoreInfoCount = 1;
    submit_info.pSignalSemaphoreInfos = &signal;
    submit_info.commandBufferInfoCount = 1;
    submit_info.pCommandBufferInfos = &command_buffer_info;

    vkQueueSubmit2(Resources::get().getGraphicsQueue(), 1, &submit_info,
                   frame_in_flight.finished_processing.fence);

    swap_chain.present();
}

void RenderEngineImpl::reinitWindowDependentResources() {
    auto config = Config::App::get().getGraphicsConfig();

    width = config.render_width;
    height = config.render_height;

    swap_chain = SwapChain(config.render_width, config.render_height,
                           config.buffering_mode);

    render_target_texture =
        ImageBuilder(VK_FORMAT_R8G8B8A8_SRGB, config.render_width,
                     config.render_height)
            .isCopySource()
            .isRenderTarget()
            .create()
            .getResult();

    render_enviroment = RenderEnviroment{};
    render_enviroment.render_target =
        RenderTarget::create(render_target_texture);
    render_enviroment.clear_render_target = true;
    render_enviroment.render_target_clear_value =
        VkClearValue{.color = {0, 0, 0, 0}};
}

void RenderEngineImpl::waitRenderFinished() {
    ZoneScoped;
    auto finished_processing =
        frames_in_flight[frame_in_flight_index].finished_processing;

    finished_processing.wait();
    finished_processing.reset();
}

void RenderEngineImpl::prepareRenderTargetForRendering(
    const CommandBuffer& cmd) {
    ZoneScoped;
    auto barrier = render_target_texture.createBarrier(
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_PIPELINE_STAGE_2_NONE,
        VK_ACCESS_2_NONE, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT |
            VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT);

    cmd.barrier(&barrier, 1);
}

void RenderEngineImpl::copyToBackbuffer(const CommandBuffer& cmd,
                                        Image& render_target,
                                        Image& backbuffer) {
    ZoneScoped;
    VkImageMemoryBarrier2 barriers[2] = {};
    barriers[0] = backbuffer.createBarrier(
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_2_NONE,
        VK_ACCESS_2_NONE, VK_PIPELINE_STAGE_2_TRANSFER_BIT,
        VK_ACCESS_2_TRANSFER_WRITE_BIT);
    barriers[1] = render_target.createBarrier(
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_2_TRANSFER_BIT, VK_ACCESS_2_TRANSFER_READ_BIT);

    cmd.barrier(barriers, 2);
    cmd.copy(render_target, backbuffer);
}

void RenderEngineImpl::prepareBackbufferForPresentation(
    const CommandBuffer& cmd, Image& backbuffer) {
    ZoneScoped;
    auto render_finished = backbuffer.createBarrier(
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_2_TRANSFER_BIT,
        VK_ACCESS_2_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_2_NONE,
        VK_ACCESS_2_NONE);

    cmd.barrier(&render_finished, 1);
}

uint32_t RenderEngineImpl::getWidth() const { return width; }

uint32_t RenderEngineImpl::getHeight() const { return height; }

}  // namespace Graphics