#include "RenderPass.h"

#include "BufferBuilder.h"
#include "CameraData.h"
#include "CloudsData.h"
#include "CloudsRenderer.h"
#include "DescriptorSet.h"
#include "GraphicsCommunicationManager.h"

namespace Graphics {

RenderPass::RenderPass(DescriptorSet& descriptors)
    : star_renderer(descriptors),
      clouds_renderer(descriptors),
      descriptors(descriptors) {
    camera_data = BufferBuilder(sizeof(CameraData))
                      .isConstantBuffer()
                      .isCPUWritable()
                      .create()
                      .getResult();
}

void RenderPass::render(const FrameData& frame_data) {
    CameraData data = {};
    data.view_projection = Matrix::projection(1.04, 16.f / 9, 1000, 1);
    data.view_projection.m[1][1] *= -1;
    data.inverse_view_projection = data.view_projection.inverse();
    void* mapped_buffer = camera_data.map();
    memcpy(mapped_buffer, &data, sizeof(CameraData));
    camera_data.unmap();

    auto& manager = GraphicsCommunicationManager::get();

    CloudsData clouds_data = {};
    clouds_data.color = {1, 0, 1};
    clouds_renderer.preRender(frame_data, camera_data, clouds_data);

    beginPass(frame_data);

    auto stars = manager.recieve<StarsData>();
    if (stars) {
        star_renderer.render(frame_data, camera_data, *stars);
    }

    clouds_renderer.render(frame_data, camera_data, clouds_data);

    endPass(frame_data);
}

void RenderPass::beginPass(const FrameData& frame_data) {
    frame_data.cmd.bindRenderEnviroment(frame_data.env);
}

void RenderPass::endPass(const FrameData& frame_data) {
    frame_data.cmd.unbindRenderEnviroment();
}

}  // namespace Graphics
