#include "CustomRenderer.h"

#include "FrameGraph.h"
#include "Graphics.h"

namespace Graphics {

CustomRenderer::CustomRenderer(Device& device, const EngineData& engine_data,
                               BufferHandle camera_buffer)
    : engine_data(engine_data), camera_buffer(camera_buffer) {}

void CustomRenderer::render(FrameGraph& frame_graph, RenderWorld& world) {
    auto render_objects = world.getCustomRenderObjects();

    for (const auto& object : render_objects) {
        auto pass = GraphicsPass(
            "", object->pipeline,
            [this, &world, object](GraphicsPassExecution& execution) {
                execution.appendData(camera_buffer->getDeviceAddress());
                execution.appendData(object->buffer->getDeviceAddress());
                execution.appendData(object->model);

                execution.draw(object->mesh);
            });

        auto render_target = engine_data.resource_manager.getTexture("Color");
        pass.addColorAttachment(*render_target);
        auto depth = engine_data.resource_manager.getTexture("Depth");
        pass.setDepthAttachment(*depth);

        frame_graph.addGraphicsPass(pass);
    }
}

}  // namespace Graphics