#pragma once

#include <vulkan/vulkan.h>

#include <cstdint>
#include <memory>

#include "CommandBuffer.h"
#include "CommandPool.h"
#include "DescriptorSet.h"
#include "Device.h"
#include "DeviceProperties.h"
#include "EngineData.h"
#include "IRenderEngine.h"
#include "Queue.h"
#include "RenderEnviroment.h"
#include "RenderPass.h"
#include "Semaphore.h"
#include "ShaderRegistry.h"
#include "StarRenderer.h"
#include "SwapChain.h"
#include "VkBootstrap.h"
// #include "Texture.h"

namespace Graphics {

class RenderEngine final : public IRenderEngine {
    static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;

    struct FrameInFlight {
        FrameInFlight(Device& device, uint32_t queue_index)
            : pool(device, queue_index),
              backbuffer_ready_for_rendering(device.createSemaphore()),
              finished_processing(device.createFence(true)) {}

        CommandPool pool;

        Semaphore backbuffer_ready_for_rendering;
        VkFence finished_processing;
    };

   public:
    RenderEngine(const vkb::Instance& instance, const vkb::Device& device,
                 const Queue& graphics_queue, const Queue& presentation_queue,
                 const VmaAllocator& allocator, VkSurfaceKHR surface);
    ~RenderEngine();

    void reinitWindowDependentResources();
    void render();

    uint32_t getWidth() const;
    uint32_t getHeight() const;

    EngineData getEngineData();

   private:
    void beginFrame(const CommandBuffer& cmd);
    void endFrame(const CommandBuffer& cmd);

    void waitRenderFinished();
    void prepareRenderTargetForRendering(const CommandBuffer& cmd);

    void copyToBackbuffer(const CommandBuffer& cmd, Image& render_target,
                          Image& backbuffer);
    void prepareBackbufferForPresentation(const CommandBuffer& cmd,
                                          Image& backbuffer);

    Device device;
    Queue graphics_queue;
    Queue presentation_queue;
    VkSurfaceKHR surface;

    DescriptorSet descriptor_set;
    ShaderRegistry shader_registry;

    Image render_target_texture;
    RenderEnviroment render_enviroment;

    std::unique_ptr<RenderPass> render_pass;

    SwapChain swap_chain;

    TracyVkCtx trace_ctx;
    int trace_dump_counter = 0;

    uint32_t width;
    uint32_t height;

    FrameInFlight frames_in_flight[MAX_FRAMES_IN_FLIGHT];
    uint32_t frame_in_flight_index;
};

}  // namespace Graphics