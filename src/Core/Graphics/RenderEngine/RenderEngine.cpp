#include "RenderEngine.h"

#include <VkBootstrap.h>
#include <vulkan/vulkan.h>

#include <string_view>
#include <tracy/Tracy.hpp>
#include <tracy/TracyVulkan.hpp>

#include "EngineData.h"
#include "FrameData.h"
#include "FrameGraph.h"
#include "MeshBuilder.h"
#include "MeshRegistry.h"
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
      shader_registry(this->backend.getDevice()),
      mesh_registry(),
      staging_buffer(this->backend.getDevice()) {}

void RenderEngine::render() {
    ZoneScoped;

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

TextureHandle RenderEngine::addTexture(void* data, uint32_t width,
                                       uint32_t height) {
    return addTexture("", data, width, height);
}

TextureHandle RenderEngine::addTexture(std::string_view name, void* data,
                                       uint32_t width, uint32_t height) {
    if (data == nullptr) return {};

    auto builder = TextureBuilder(VK_FORMAT_R8G8B8A8_SRGB, width, height)
                       .isShaderResource()
                       .isCopyDestination();
    if (name != "") builder.setName(name);

    auto image = builder.create(backend.getDevice()).getResult();

    if (name != "") resource_manager.addTexture(name, image);

    staging_buffer.stageTexture(image, data, width * height * 4);

    return image;
}

MeshHandle RenderEngine::addMesh(const ::Mesh& mesh) {
    auto device_mesh =
        MeshBuilder(mesh).create(backend.getDevice(), staging_buffer);

    auto handle = mesh_registry.addMesh(device_mesh);

    return handle;
}

MeshHandle RenderEngine::addMesh(std::string_view name, const ::Mesh& mesh) {
    auto device_mesh =
        MeshBuilder(mesh).create(backend.getDevice(), staging_buffer);

    auto handle = mesh_registry.addMesh(std::string(name), device_mesh);

    return handle;
}

EngineData RenderEngine::getEngineData() {
    return EngineData{shader_registry, mesh_registry, staging_buffer,
                      resource_manager};
}

RenderWorld& RenderEngine::getRenderWorld() { return world; }

}  // namespace Graphics