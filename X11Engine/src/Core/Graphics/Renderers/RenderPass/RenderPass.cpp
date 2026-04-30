#include "RenderPass.h"

#include "BufferBuilder.h"
#include "CameraData.h"
#include "GraphicsCommunicationManager.h"

namespace Graphics {

RenderPass::RenderPass() {
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

    beginPass(cmd, env);

    auto stars = manager.recieve<StarsData>();
    if (stars) {
        star_renderer.render(cmd, camera_data, *stars);
    }

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
