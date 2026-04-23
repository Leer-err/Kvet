#include "Renderer.h"

#include <vulkan/vulkan_core.h>

#include <tracy/Tracy.hpp>

// #include "AppConfig.h"
// #include "Context.h"
// #include "Format.h"
#include "AppConfig.h"
#include "Context.h"
#include "GraphicsResources.h"
#include "ImageBarrier.h"
#include "InternalSwapChain.h"
#include "RenderEnviroment.h"
#include "SwapChainBuilder.h"
#include "TextureBuilder.h"
#include "Vector4.h"

namespace Graphics {

Renderer::Renderer() { Resources::get(); }

void Renderer::beginFrame() {
    ZoneScoped;

    Resources::get().swapFrame();
    auto frame = Resources::get().getFrameInFlight();
    auto device = Resources::get().getDevice();

    vkWaitForFences(device, 1, &frame.render_finished.fence, true, UINT64_MAX);
    vkResetFences(device, 1, &frame.render_finished.fence);

    frame.buffer.begin();

    Internal::ImageBarrier barrier_builder(
        render_target_texture.getInternal()->image,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    barrier_builder.src_stage_mask =
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    barrier_builder.dst_stage_mask =
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    barrier_builder.dst_access_mask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                                      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    auto barrier = barrier_builder.create();

    VkDependencyInfo dep = {};
    dep.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dep.imageMemoryBarrierCount = 1;
    dep.pImageMemoryBarriers = &barrier;

    vkCmdPipelineBarrier2(frame.buffer.buffer, &dep);

    auto render_enviroment = RenderEnviroment{};
    render_enviroment.render_target = default_render_target;
    render_enviroment.clear_render_target = true;
    render_enviroment.render_target_clear_value = Vector4();

    auto context = Context();
    context.bindRenderEnviroment(render_enviroment);
}

void Renderer::endFrame() {
    ZoneScoped;

    auto context = Context();
    context.unbindRenderEnviroment();

    auto frame = Resources::get().getFrameInFlight();

    auto backbuffer = swap_chain.getBackbuffer();

    VkImageMemoryBarrier2 barriers[2] = {};

    Internal::ImageBarrier backbuffer_barrier_builder(
        backbuffer.getInternal()->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    backbuffer_barrier_builder.dst_stage_mask =
        VK_PIPELINE_STAGE_2_TRANSFER_BIT;
    backbuffer_barrier_builder.dst_access_mask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
    barriers[0] = backbuffer_barrier_builder.create();

    Internal::ImageBarrier render_target_barrier_builder(
        render_target_texture.getInternal()->image,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    render_target_barrier_builder.src_stage_mask =
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    render_target_barrier_builder.src_access_mask =
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
    render_target_barrier_builder.dst_stage_mask =
        VK_PIPELINE_STAGE_2_TRANSFER_BIT;
    render_target_barrier_builder.dst_access_mask =
        VK_ACCESS_2_TRANSFER_READ_BIT;
    barriers[1] = render_target_barrier_builder.create();

    VkDependencyInfo dep = {};
    dep.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dep.imageMemoryBarrierCount = 2;
    dep.pImageMemoryBarriers = barriers;

    vkCmdPipelineBarrier2(frame.buffer.buffer, &dep);

    VkImageCopy2 copy_region = {};
    copy_region.sType = VK_STRUCTURE_TYPE_IMAGE_COPY_2;
    copy_region.extent.width = 1280;
    copy_region.extent.height = 720;
    copy_region.extent.depth = 1;
    copy_region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy_region.srcSubresource.layerCount = 1;
    copy_region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy_region.dstSubresource.layerCount = 1;

    VkCopyImageInfo2 copy_info = {};
    copy_info.sType = VK_STRUCTURE_TYPE_COPY_IMAGE_INFO_2;
    copy_info.srcImage = render_target_texture.getInternal()->image;
    copy_info.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    copy_info.dstImage = backbuffer.getInternal()->image;
    copy_info.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    copy_info.regionCount = 1;
    copy_info.pRegions = &copy_region;

    vkCmdCopyImage2(frame.buffer.buffer, &copy_info);

    Internal::ImageBarrier barrier_builder(backbuffer.getInternal()->image,
                                           VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    barrier_builder.src_stage_mask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
    barrier_builder.src_access_mask = VK_ACCESS_2_TRANSFER_WRITE_BIT;

    auto barrier = barrier_builder.create();

    dep = {};
    dep.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dep.imageMemoryBarrierCount = 1;
    dep.pImageMemoryBarriers = &barrier;

    vkCmdPipelineBarrier2(frame.buffer.buffer, &dep);

    frame.buffer.end();

    VkCommandBufferSubmitInfo command_buffer_info = {};
    command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
    command_buffer_info.commandBuffer = frame.buffer.buffer;
    command_buffer_info.deviceMask = 0;

    VkSemaphoreSubmitInfo semaphore_info = {};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    semaphore_info.semaphore =
        swap_chain.getInternal()
            ->semaphores[swap_chain.getInternal()->image_index]
            .semaphore;

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

    // context.copy(render_target_texture, backbuffer);

    swap_chain.present();
}

void Renderer::initializeResources() {
    auto config = Config::App::get().getGraphicsConfig();

    width = config.render_width;
    height = config.render_height;

    swap_chain = SwapChainBuilder(config.render_width, config.render_height,
                                  config.buffering_mode)
                     .create();

    render_target_texture =
        TextureBuilder(Texture::Format::RGBA8_SRGB, config.render_width,
                       config.render_height)
            .isCopySource()
            .isRenderTarget()
            .create()
            .getResult();

    auto context = Context();
    default_render_target = context.createRenderTarget(render_target_texture);

    // auto context = APIResources::get().getContext();
    // auto device = APIResources::get().getDevice();

    // D3D11_BLEND_DESC1 desc = {};
    // desc.RenderTarget[0].BlendEnable = TRUE;
    // desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    // desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    // desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    // desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    // desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    // desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    // desc.RenderTarget[0].RenderTargetWriteMask =
    // D3D11_COLOR_WRITE_ENABLE_ALL;

    // Microsoft::WRL::ComPtr<ID3D11BlendState1> blend_state;
    // device->CreateBlendState1(&desc, &blend_state);

    // context->OMSetBlendState(blend_state.Get(), nullptr, 0xffffffff);
}

uint32_t Renderer::getWidth() const { return width; }

uint32_t Renderer::getHeight() const { return height; }

}  // namespace Graphics