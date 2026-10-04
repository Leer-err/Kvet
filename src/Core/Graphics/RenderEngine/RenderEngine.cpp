#include "RenderEngine.h"

#include <VkBootstrap.h>
#include <vulkan/vulkan.h>

#include <string_view>
#include <tracy/Tracy.hpp>
#include <tracy/TracyVulkan.hpp>

#include "EngineData.h"
#include "FrameData.h"
#include "FrameGraph.h"
#include "Graphics.h"
#include "MeshBuilder.h"
#include "RenderWorld.h"
#include "StagingBuffer.h"
#include "Texture.h"
#include "TextureBuilder.h"

namespace Graphics {

RenderEngine::RenderEngine(const vkb::Instance& instance,
                           const vkb::Device& device,
                           const Queue& graphics_queue,
                           const Queue& presentation_queue,
                           const VmaAllocator& allocator, VkSurfaceKHR surface)
    : backend(instance, device, graphics_queue, presentation_queue, allocator,
              surface),
      staging_buffer(this->backend.getDevice()),
      resource_manager(this->backend.getDevice(), staging_buffer),
      world(this->backend.getDevice()) {}

void RenderEngine::render() {
    ZoneScoped;
    world.update();

    if (!render_pass)
        render_pass = std::make_unique<RenderPass>(this->backend.getDevice(),
                                                   getEngineData());

    FrameData frame = backend.beginFrame();

    FrameGraph frame_graph(getEngineData());

    staging_buffer.flush(frame.cmd);
    auto rendered_image = render_pass->render(frame, frame_graph, world);

    frame_graph.execute(frame);

    backend.endFrame(rendered_image);
}

EngineData RenderEngine::getEngineData() {
    return EngineData{staging_buffer, resource_manager};
}

const IRenderWorld* RenderEngine::getRenderWorld() const { return &world; }

IRenderWorld* RenderEngine::getRenderWorld() { return &world; }

const IResourceManager* RenderEngine::getResourceManager() const {
    return &resource_manager;
}

IResourceManager* RenderEngine::getResourceManager() {
    return &resource_manager;
}

}  // namespace Graphics