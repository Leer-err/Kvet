#include "StaticMeshRenderer.h"

#include <vulkan/vulkan_core.h>

#include "BufferBuilder.h"
#include "GraphicsPipelineBuilder.h"
#include "Matrix.h"
#include "MeshHandle.h"
#include "TextureHandle.h"

namespace Graphics {

StaticMeshRenderer::StaticMeshRenderer(const EngineData& engine_data)
    : engine_data(engine_data), model_data_buffer(this->engine_data) {
    pipeline = GraphicsPipelineBuilder(
                   engine_data, "./Assets/Shaders/StaticModel/StaticModel.spv",
                   "vertex_main",
                   "./Assets/Shaders/StaticModel/StaticModel.spv", "pixel_main")
                   .create()
                   .getResult();
}

void StaticMeshRenderer::render(const FrameData& frame_data,
                                const StaticModelData& model_data) {
    TracyVkZone(frame_data.trace_ctx, frame_data.cmd.buffer, "Static mesh");

    auto command_buffer = frame_data.cmd;

    StaticModelBuffer buffer = {};
    buffer.model = Matrix::translation(model_data.position);
    buffer.albedo_descriptor = model_data.albedo;

    model_data_buffer.update(frame_data, buffer);
    push_constants.model_data = model_data_buffer.getAddress(frame_data);

    command_buffer.setPipeline(pipeline);

    command_buffer.pushConstants(pipeline, &push_constants);
    auto mesh = engine_data.mesh_registry.getMesh(model_data.mesh);
    if (mesh) {
        command_buffer.draw(*mesh);
    }
}

void StaticMeshRenderer::setCameraData(const VkDeviceAddress camera_data) {
    push_constants.camera_data = camera_data;
}

}  // namespace Graphics