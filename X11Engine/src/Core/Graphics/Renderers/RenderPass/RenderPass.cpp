#include "RenderPass.h"

#include <tracy/Tracy.hpp>

#include "BufferBuilder.h"
#include "CameraData.h"
#include "CloudsData.h"
#include "CloudsRenderer.h"
#include "Device.h"
#include "GraphicsCommunicationManager.h"
#include "OverlayRenderer.h"
#include "StaticModelData.h"

namespace Graphics {

RenderPass::RenderPass(EngineData engine_data)
    : engine_data(engine_data),
      star_renderer(engine_data),
      clouds_renderer(engine_data),
      static_mesh_renderer(engine_data),
      overlay_renderer(engine_data) {
    camera_data_buffer = BufferBuilder(engine_data, sizeof(CameraData))
                             .isConstantBuffer()
                             .isCPUWritable()
                             .create()
                             .getResult();

    clouds_renderer.setCameraData(camera_data_buffer.device_address);
    star_renderer.setCameraData(camera_data_buffer.device_address);
    static_mesh_renderer.setCameraData(camera_data_buffer.device_address);
}

void RenderPass::render(const FrameData& frame_data) {
    ZoneScoped;
    auto& manager = GraphicsCommunicationManager::get();

    TracyVkZone(frame_data.trace_ctx, frame_data.cmd.buffer, "Render pass");

    auto clouds = manager.recieve<CloudsData>();

    if (clouds) {
        clouds_renderer.preRender(frame_data, clouds.value());
    }

    updateCameraBuffer();

    beginPass(frame_data);

    auto stars = manager.recieve<StarsData>();
    if (stars) {
        star_renderer.render(frame_data, camera_data_buffer, *stars);
    }
    if (clouds) {
        clouds_renderer.render(frame_data, clouds.value());
    }

    while (auto model = manager.recieve<StaticModelData>()) {
        static_mesh_renderer.render(frame_data, model.value());
    }

    overlay_renderer.render(frame_data);

    endPass(frame_data);
}

void RenderPass::beginPass(const FrameData& frame_data) {
    frame_data.cmd.bindRenderEnviroment(frame_data.env);
}

void RenderPass::endPass(const FrameData& frame_data) {
    frame_data.cmd.unbindRenderEnviroment();
}

void RenderPass::updateCameraBuffer() {
    // auto camera_data =
    //     GraphicsCommunicationManager::get().recieve<CameraData>();

    // if (!camera_data) return;

    CameraData data = {};
    data.view_projection = Matrix::projection(1.04, 16.f / 9, 1000, 1);
    data.view_projection.m[1][1] *= -1;
    data.inverse_view_projection = data.view_projection.inverse();

    void* mapped_buffer = engine_data.device.map(camera_data_buffer);
    memcpy(mapped_buffer, &data, sizeof(CameraData));
    engine_data.device.unmap(camera_data_buffer);
}

}  // namespace Graphics
