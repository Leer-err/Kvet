#include "StarRenderer.h"

#include <vulkan/vulkan.h>

#include <cstring>

#include "BufferBuilder.h"
#include "CommandBuffer.h"
#include "EngineData.h"
#include "GraphicsPipelineBuilder.h"
#include "StarsData.h"
#include "Vector3.h"

namespace Graphics {

StarRenderer::StarRenderer(const EngineData& engine_data)
    : engine_data(engine_data) {
    constexpr Vector3 screen_quad_vertices[] = {
        Vector3(-1, -1, 1), Vector3(1, -1, 1), Vector3(-1, 1, 1),
        Vector3(1, 1, 1)};

    constexpr uint32_t screen_quad_indices[] = {0, 1, 2, 1, 3, 2};

    quad_vertices = BufferBuilder(engine_data, sizeof(screen_quad_vertices))
                        .isVertexBuffer(sizeof(Vector3))
                        .isCPUWritable()
                        .create()
                        .getResult();
    auto vertex_data = engine_data.device.map(quad_vertices);
    memcpy(vertex_data, screen_quad_vertices, sizeof(screen_quad_vertices));
    engine_data.device.unmap(quad_vertices);

    quad_indices = BufferBuilder(engine_data, sizeof(screen_quad_indices))
                       .isIndexBuffer()
                       .isCPUWritable()
                       .create()
                       .getResult();
    auto index_data = engine_data.device.map(quad_indices);
    memcpy(index_data, screen_quad_indices, sizeof(screen_quad_indices));
    engine_data.device.unmap(quad_indices);

    pipeline =
        GraphicsPipelineBuilder(
            engine_data, "./Assets/Shaders/Stars/Stars.spv", "vertex_main",
            "./Assets/Shaders/Stars/Stars.spv", "pixel_main")
            .create()
            .getResult();

    stars_data_buffer = BufferBuilder(engine_data, sizeof(StarsData))
                            .isConstantBuffer()
                            .isCPUWritable()
                            .create()
                            .getResult();
    push_constants.stars_data = stars_data_buffer.device_address;
}

void StarRenderer::render(const FrameData& frame_data,
                          const Buffer& camera_data,
                          const StarsData& stars_data) {
    TracyVkZone(frame_data.trace_ctx, frame_data.cmd.buffer, "Stars");

    auto command_buffer = frame_data.cmd;

    auto stars_data_ptr = engine_data.device.map(stars_data_buffer);
    memcpy(stars_data_ptr, &stars_data, sizeof(stars_data));
    engine_data.device.unmap(stars_data_buffer);

    command_buffer.setPipeline(pipeline);

    command_buffer.pushConstants(pipeline, &push_constants);

    command_buffer.draw(quad_vertices, quad_indices);
}

void StarRenderer::setCameraData(VkDeviceAddress camera_data) {
    push_constants.camera_data = camera_data;
}

}  // namespace Graphics