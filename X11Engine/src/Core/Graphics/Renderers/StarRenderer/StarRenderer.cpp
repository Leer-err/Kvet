#include "StarRenderer.h"

#include <vulkan/vulkan_core.h>

#include <cstring>

#include "BufferBuilder.h"
#include "CameraData.h"
#include "CommandBuffer.h"
#include "GraphicsPipelineBuilder.h"
#include "InputLayoutBuilder.h"
#include "Shader.h"
#include "ShaderBuilder.h"
#include "StarsData.h"
#include "Vector3.h"

namespace Graphics {

struct Constants {
    VkDeviceAddress camera_data;
    VkDeviceAddress stars_data;
};

StarRenderer::StarRenderer() {
    constexpr Vector3 screen_quad_vertices[] = {
        Vector3(-1, -1, 1), Vector3(1, -1, 1), Vector3(-1, 1, 1),
        Vector3(1, 1, 1)};

    constexpr uint32_t screen_quad_indices[] = {0, 1, 2, 1, 3, 2};

    quad_vertices = BufferBuilder(sizeof(screen_quad_vertices))
                        .isVertexBuffer(sizeof(Vector3))
                        .isCPUWritable()
                        .create()
                        .getResult();
    auto vertex_data = quad_vertices.map();
    memcpy(vertex_data, screen_quad_vertices, sizeof(screen_quad_vertices));
    quad_vertices.unmap();

    quad_indices = BufferBuilder(sizeof(screen_quad_indices))
                       .isIndexBuffer()
                       .isCPUWritable()
                       .create()
                       .getResult();
    auto index_data = quad_indices.map();
    memcpy(index_data, screen_quad_indices, sizeof(screen_quad_indices));
    quad_indices.unmap();

    pipeline = GraphicsPipelineBuilder(
                   "./Assets/Shaders/Stars/Stars.spv", "vertex_main",
                   "./Assets/Shaders/Stars/Stars.spv", "pixel_main")
                   .create()
                   .getResult();

    stars_data_buffer = BufferBuilder(sizeof(StarsData))
                            .isConstantBuffer()
                            .isCPUWritable()
                            .create()
                            .getResult();
}

void StarRenderer::render(const CommandBuffer& command_buffer,
                          const Buffer& camera_data,
                          const StarsData& stars_data) {
    auto stars_data_ptr = stars_data_buffer.map();
    memcpy(stars_data_ptr, &stars_data, sizeof(stars_data));
    stars_data_buffer.unmap();

    command_buffer.setPipeline(pipeline);

    VkDeviceAddress camera_address = camera_data.getDeviceAddress();
    VkDeviceAddress stars_address = stars_data_buffer.getDeviceAddress();
    auto constants = Constants{camera_address, stars_address};
    command_buffer.pushConstants(pipeline, &constants);

    command_buffer.draw(quad_vertices, quad_indices);
}

}  // namespace Graphics