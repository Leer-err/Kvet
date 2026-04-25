#include "RendererImpl.h"

#include <vulkan/vulkan_core.h>

#include <cstring>
#include <tracy/Tracy.hpp>

// #include "AppConfig.h"
// #include "Context.h"
// #include "Format.h"
#include "AppConfig.h"
// #include "Context.h"
#include "GraphicsCommunicationManager.h"
#include "GraphicsResources.h"
// #include "RenderEnviroment.h"
#include "BufferBuilder.h"
#include "CameraData.h"
#include "ImageBuilder.h"
#include "RenderTarget.h"
#include "StarsData.h"
#include "SwapChain.h"

namespace Graphics {

RendererImpl::RendererImpl() {
    init();
    camera_data = Graphics::BufferBuilder(sizeof(CameraData))
                      .isConstantBuffer()
                      .isCPUWritable()
                      .create()
                      .getResult();
}

RendererImpl::~RendererImpl() { swap_chain.destroy(); }

void RendererImpl::render() {
    ZoneScoped;
    auto& manager = GraphicsCommunicationManager::get();
    // auto data = manager.recieve<CameraData>();
    CameraData data = {};
    data.view_projection = Matrix::identity();
    data.inverse_view_projection = Matrix::identity();

    auto stars = manager.recieve<StarsData>();

    void* mapped_buffer = camera_data.map();
    memcpy(mapped_buffer, &data, sizeof(CameraData));
    camera_data.unmap();

    beginFrame();

    if (stars) {
        auto frame = Resources::get().getFrameInFlight();
        star_renderer.render(frame.buffer, camera_data, *stars);
    }

    endFrame();
}

void RendererImpl::beginFrame() {
    ZoneScoped;

    Resources::get().swapFrame();

    waitRenderFinished();

    auto frame = Resources::get().getFrameInFlight();
    frame.buffer.begin();

    prepareRenderTargetForRendering();

    auto render_enviroment = RenderEnviroment{};
    render_enviroment.render_target = render_target;
    render_enviroment.clear_render_target = true;
    render_enviroment.render_target_clear_value =
        VkClearValue{.color = {0, 0, 0, 0}};

    frame.buffer.bindRenderEnviroment(render_enviroment);
}

void RendererImpl::endFrame() {
    ZoneScoped;

    auto frame = Resources::get().getFrameInFlight();
    frame.buffer.unbindRenderEnviroment();

    auto [backbuffer, ready_for_present] = swap_chain.getBackbuffer();

    copyToBackbuffer(render_target_texture, backbuffer);
    prepareBackbufferForPresentation(backbuffer);

    frame.buffer.end();

    VkCommandBufferSubmitInfo command_buffer_info = {};
    command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
    command_buffer_info.commandBuffer = frame.buffer.buffer;
    command_buffer_info.deviceMask = 0;

    auto signal = ready_for_present.submit();
    auto wait = frame.ready_for_render.submit();

    VkSubmitInfo2 submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    submit_info.waitSemaphoreInfoCount = 1;
    submit_info.pWaitSemaphoreInfos = &wait;
    submit_info.signalSemaphoreInfoCount = 1;
    submit_info.pSignalSemaphoreInfos = &signal;
    submit_info.commandBufferInfoCount = 1;
    submit_info.pCommandBufferInfos = &command_buffer_info;

    vkQueueSubmit2(Resources::get().getGraphicsQueue(), 1, &submit_info,
                   frame.render_finished.fence);

    swap_chain.present();
}

void RendererImpl::init() {
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

    render_target = RenderTarget::create(render_target_texture);
}

void RendererImpl::waitRenderFinished() {
    ZoneScoped;
    auto frame = Resources::get().getFrameInFlight();

    frame.render_finished.wait();
    frame.render_finished.reset();
}

void RendererImpl::prepareRenderTargetForRendering() {
    ZoneScoped;
    auto frame = Resources::get().getFrameInFlight();

    auto barrier = render_target_texture.createBarrier(
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_PIPELINE_STAGE_2_NONE,
        VK_ACCESS_2_NONE, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT |
            VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT);

    VkDependencyInfo dep = {};
    dep.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dep.imageMemoryBarrierCount = 1;
    dep.pImageMemoryBarriers = &barrier;

    vkCmdPipelineBarrier2(frame.buffer.buffer, &dep);
}

void RendererImpl::copyToBackbuffer(Image& render_target, Image& backbuffer) {
    ZoneScoped;
    auto frame = Resources::get().getFrameInFlight();

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

    VkDependencyInfo dep = {};
    dep.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dep.imageMemoryBarrierCount = 2;
    dep.pImageMemoryBarriers = barriers;

    vkCmdPipelineBarrier2(frame.buffer.buffer, &dep);

    frame.buffer.copy(render_target, backbuffer);
}

void RendererImpl::prepareBackbufferForPresentation(Image& backbuffer) {
    ZoneScoped;
    auto frame = Resources::get().getFrameInFlight();

    auto render_finished = backbuffer.createBarrier(
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_2_TRANSFER_BIT,
        VK_ACCESS_2_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_2_NONE,
        VK_ACCESS_2_NONE);

    VkDependencyInfo dep = {};
    dep.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dep.imageMemoryBarrierCount = 1;
    dep.pImageMemoryBarriers = &render_finished;

    vkCmdPipelineBarrier2(frame.buffer.buffer, &dep);
}

uint32_t RendererImpl::getWidth() const { return width; }

uint32_t RendererImpl::getHeight() const { return height; }

}  // namespace Graphics