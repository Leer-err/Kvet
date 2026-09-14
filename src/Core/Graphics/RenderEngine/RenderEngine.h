#pragma once

#include <vulkan/vulkan.h>

#include <cstdint>

#include "CommandBuffer.h"
#include "Device.h"
#include "EngineData.h"
#include "GraphicsResourceManager.h"
#include "Handles.h"
#include "MeshRegistry.h"
#include "Queue.h"
#include "RenderPass.h"
#include "RenderWorld.h"
#include "RenderingBackend.h"
#include "ShaderRegistry.h"
#include "StagingBuffer.h"
#include "VkBootstrap.h"

namespace Graphics {

class RenderEngine final : public IRenderEngine {
   public:
    RenderEngine(const vkb::Instance& instance, const vkb::Device& device,
                 const Queue& graphics_queue, const Queue& presentation_queue,
                 const VmaAllocator& allocator, VkSurfaceKHR surface);

    void reinitWindowDependentResources();
    void render() override;

    IRenderWorld* getRenderWorld() const override;
    IResourceManager* getResourceManager() const override;

    EngineData getEngineData();

   private:
    void beginFrame(const CommandBuffer& cmd);
    void endFrame(const CommandBuffer& cmd);

    void waitRenderFinished();

    RenderingBackend backend;

    ShaderRegistry shader_registry;
    MeshRegistry mesh_registry;

    StagingBuffer staging_buffer;

    ResourceManager resource_manager;

    RenderWorld world;

    std::unique_ptr<RenderPass> render_pass;
};

}  // namespace Graphics