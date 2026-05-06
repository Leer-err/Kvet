#pragma once

// #include "DepthStencil.h"
// #include "RenderTarget.h"
#include <cstdint>

// #include "RenderTarget.h"
#include "CloudsRenderer.h"
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "DescriptorSet/DescriptorSet.h"
#include "Fence.h"
#include "IRenderEngine.h"
#include "Queue.h"
#include "RenderEnviroment.h"
#include "RenderPass.h"
#include "RenderTarget.h"
#include "Semaphore.h"
#include "StarRenderer.h"
#include "SwapChain.h"
#include "VkBootstrap.h"
// #include "Texture.h"

namespace Graphics {

class RenderEngine final : public IRenderEngine {
    static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;

    struct FrameInFlight {
        CommandPool pool;

        Semaphore backbuffer_ready_for_rendering;
        Fence finished_processing;
    };

   public:
    RenderEngine(const vkb::Instance& instance, const vkb::Device& device,
                 const Queue& graphics_queue, const Queue& presentation_queue,
                 const VmaAllocator& allocator);
    ~RenderEngine();

    void reinitWindowDependentResources();
    void render();

    uint32_t getWidth() const;
    uint32_t getHeight() const;

   private:
    void beginFrame(const CommandBuffer& cmd);
    void endFrame(const CommandBuffer& cmd);

    void waitRenderFinished();
    void prepareRenderTargetForRendering(const CommandBuffer& cmd);

    void copyToBackbuffer(const CommandBuffer& cmd, Image& render_target,
                          Image& backbuffer);
    void prepareBackbufferForPresentation(const CommandBuffer& cmd,
                                          Image& backbuffer);

    Image render_target_texture;
    RenderEnviroment render_enviroment;

    DescriptorSet descriptor_set;

    RenderPass render_pass;

    SwapChain swap_chain;

    uint32_t width;
    uint32_t height;

    FrameInFlight frames_in_flight[MAX_FRAMES_IN_FLIGHT];
    uint32_t frame_in_flight_index;
};

}  // namespace Graphics