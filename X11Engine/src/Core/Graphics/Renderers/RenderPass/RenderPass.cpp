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

void RenderPass::render(CommandBuffer& cmd, const RenderEnviroment& env) {
    CameraData data = {};
    data.view_projection = Matrix::identity();
    data.inverse_view_projection = Matrix::identity();
    void* mapped_buffer = camera_data.map();
    memcpy(mapped_buffer, &data, sizeof(CameraData));
    camera_data.unmap();

    auto& manager = GraphicsCommunicationManager::get();

    CloudsData clouds_data = {};
    clouds_data.color = {1, 0, 1};
    clouds_renderer.preRender(cmd, camera_data, clouds_data);

    beginPass(cmd, env);

    auto stars = manager.recieve<StarsData>();
    if (stars) {
        star_renderer.render(cmd, camera_data, *stars);
    }

    clouds_renderer.render(cmd, camera_data, clouds_data);

    endPass(cmd);
}

void RenderPass::beginPass(const CommandBuffer& cmd,
                           const RenderEnviroment& env) {
    cmd.bindRenderEnviroment(env);
}

void RenderPass::endPass(const CommandBuffer& cmd) {
    cmd.unbindRenderEnviroment();
}

}  // namespace Graphics
