#include "RenderPass.h"

#include "BufferBuilder.h"
#include "CameraData.h"
#include "CloudsData.h"
#include "CloudsRenderer.h"
#include "GraphicsCommunicationManager.h"

namespace Graphics {

RenderPass::RenderPass(EngineData engine_data)
    : engine_data(engine_data),
      star_renderer(engine_data),
      clouds_renderer(engine_data) {
    camera_data_buffer = BufferBuilder(engine_data, sizeof(CameraData))
                             .isConstantBuffer()
                             .isCPUWritable()
                             .create()
                             .getResult();

    clouds_renderer.setCameraData(camera_data_buffer.device_address);
    star_renderer.setCameraData(camera_data_buffer.device_address);
}

void RenderPass::render(const FrameData& frame_data) {
    auto& manager = GraphicsCommunicationManager::get();

    CloudsData clouds_data = {};
    clouds_data.color = {1, 0, 1};
    clouds_renderer.preRender(frame_data, clouds_data);

    beginPass(frame_data);

    auto stars = manager.recieve<StarsData>();
    if (stars) {
        star_renderer.render(frame_data, camera_data_buffer, *stars);
    }

    clouds_renderer.render(frame_data, clouds_data);

    endPass(frame_data);
}

void RenderPass::beginPass(const FrameData& frame_data) {
    frame_data.cmd.bindRenderEnviroment(frame_data.env);
}

void RenderPass::endPass(const FrameData& frame_data) {
    frame_data.cmd.unbindRenderEnviroment();
}

void RenderPass::updateCameraBuffer() {
    auto camera_data =
        GraphicsCommunicationManager::get().recieve<CameraData>();

    if (!camera_data) return;

    CameraData data = {};
    data.view_projection = Matrix::projection(1.04, 16.f / 9, 1000, 1);
    data.view_projection.m[1][1] *= -1;
    data.inverse_view_projection = data.view_projection.inverse();

    void* mapped_buffer = engine_data.device.map(camera_data_buffer);
    memcpy(mapped_buffer, &camera_data, sizeof(CameraData));
    engine_data.device.unmap(camera_data_buffer);
}

}  // namespace Graphics
