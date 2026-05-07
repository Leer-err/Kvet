#include "RenderEngine.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <tracy/Tracy.hpp>

// #include "AppConfig.h"
// #include "Context.h"
// #include "Format.h"
#include "AppConfig.h"
// #include "Context.h"
#include "CommandBuffer.h"
#include "DescriptorSet.h"
#include "DeviceProperties.h"
#include "EngineData.h"
#include "Fence.h"
#include "FrameData.h"
// #include "RenderEnviroment.h"
#include "ImageBuilder.h"
#include "RenderPass.h"
#include "ShaderRegistry.h"
#include "SwapChain.h"
#include "VkBootstrap.h"

namespace Graphics {

RenderEngine::RenderEngine(const vkb::Instance& instance,
                           const vkb::Device& device,
                           const Queue& graphics_queue,
                           const Queue& presentation_queue,
                           const VmaAllocator& allocator, VkSurfaceKHR surface)
    : instance(instance),
      device(device, allocator),
      shader_registry(this->device),
      descriptor_set(this->device, this->device.getDeviceProperties()),
      graphics_queue(graphics_queue),
      presentation_queue(presentation_queue),
      surface(surface),
      frame_in_flight_index(0) {
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        auto& frame_in_flight = frames_in_flight[i];
        frame_in_flight.pool = CommandPool::create();
        frame_in_flight.backbuffer_ready_for_rendering = Semaphore::create();
        frame_in_flight.finished_processing = Fence::create(true);
    }

    reinitWindowDependentResources();
}

RenderEngine::~RenderEngine() {
    device.waitIdle();

    swap_chain.destroy();
    vkb::destroy_surface(instance, surface);
    vkb::destroy_instance(instance);
}

void RenderEngine::render() {
    ZoneScoped;

    waitRenderFinished();

    auto pool = frames_in_flight[frame_in_flight_index].pool;
    auto cmd = pool.getCommandBuffer();

    beginFrame(cmd);

    FrameData data = {};
    data.cmd = cmd;
    data.env = render_enviroment;

    render_pass->render(data);

    endFrame(cmd);

    frame_in_flight_index = (frame_in_flight_index + 1) % MAX_FRAMES_IN_FLIGHT;
}

void RenderEngine::beginFrame(const CommandBuffer& cmd) {
    ZoneScoped;

    cmd.begin();
    prepareRenderTargetForRendering(cmd);
}

void RenderEngine::endFrame(const CommandBuffer& cmd) {
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

    vkQueueSubmit2(graphics_queue.queue, 1, &submit_info,
                   frame_in_flight.finished_processing.fence);

    swap_chain.present();
}

void RenderEngine::reinitWindowDependentResources() {
    auto config = Config::App::get().getGraphicsConfig();

    width = config.render_width;
    height = config.render_height;

    swap_chain = SwapChain(device, presentation_queue, config.render_width,
                           config.render_height, config.buffering_mode);

    render_target_texture =
        ImageBuilder(device, VK_FORMAT_R8G8B8A8_SRGB, config.render_width,
                     config.render_height)
            .isCopySource()
            .isRenderTarget()
            .create()
            .getResult();

    render_enviroment = RenderEnviroment{};
    render_enviroment.render_target =
        device.createRenderTarget(render_target_texture);
    render_enviroment.clear_render_target = true;
    render_enviroment.render_target_clear_value =
        VkClearValue{.color = {0, 0, 0, 1}};
}

void RenderEngine::waitRenderFinished() {
    ZoneScoped;
    auto finished_processing =
        frames_in_flight[frame_in_flight_index].finished_processing;

    finished_processing.wait();
    finished_processing.reset();
}

void RenderEngine::prepareRenderTargetForRendering(const CommandBuffer& cmd) {
    ZoneScoped;
    auto barrier = render_target_texture.createBarrier(
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_PIPELINE_STAGE_2_NONE,
        VK_ACCESS_2_NONE, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT |
            VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT);

    cmd.barrier(&barrier, 1);
}

void RenderEngine::copyToBackbuffer(const CommandBuffer& cmd,
                                    Image& render_target, Image& backbuffer) {
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

void RenderEngine::prepareBackbufferForPresentation(const CommandBuffer& cmd,
                                                    Image& backbuffer) {
    ZoneScoped;
    auto render_finished = backbuffer.createBarrier(
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_2_TRANSFER_BIT,
        VK_ACCESS_2_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_2_NONE,
        VK_ACCESS_2_NONE);

    cmd.barrier(&render_finished, 1);
}

EngineData RenderEngine::getEngineData() {
    return EngineData{device, descriptor_set, shader_registry};
}

uint32_t RenderEngine::getWidth() const { return width; }

uint32_t RenderEngine::getHeight() const { return height; }

}  // namespace Graphics