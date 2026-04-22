#include "Renderer.h"

#include <tracy/Tracy.hpp>

// #include "AppConfig.h"
// #include "Context.h"
// #include "Format.h"
#include "AppConfig.h"
#include "Context.h"
#include "GraphicsResources.h"
#include "ImageBarrier.h"
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

    auto context = Context();
    auto render_target = context.createRenderTarget(render_target_texture);
    auto render_enviroment = RenderEnviroment{};
    render_enviroment.render_target = render_target;
    render_enviroment.clear_render_target = true;
    render_enviroment.render_target_clear_value = Vector4();

    context.bindRenderEnviroment(render_enviroment);

    // context.clean(default_render_target);
}

void Renderer::endFrame() {
    ZoneScoped;

    auto context = Context();
    context.unbindRenderEnviroment();

    auto frame = Resources::get().getFrameInFlight();

    auto backbuffer = swap_chain.getBackbuffer();

    Internal::ImageBarrier barrier_builder(backbuffer.getInternal()->image,
                                           VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    barrier_builder.src_stage_mask =
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    barrier_builder.src_access_mask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;

    auto barrier = barrier_builder.create();

    VkDependencyInfo dep = {};
    dep.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dep.imageMemoryBarrierCount = 1;
    dep.pImageMemoryBarriers = &barrier;

    vkCmdPipelineBarrier2(frame.buffer.buffer, &dep);

    frame.buffer.end();

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
        TextureBuilder(Texture::Format::RGBA8, config.render_width,
                       config.render_height)
            .isCopyDestination()
            .isRenderTarget()
            .create()
            .getResult();

    // default_render_target = RenderTarget(render_target_texture);

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