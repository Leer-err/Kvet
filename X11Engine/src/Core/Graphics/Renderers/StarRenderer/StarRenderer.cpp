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
#include "Vector3.h"

namespace Graphics {

StarRenderer::StarRenderer() {
    constexpr Vector3 screen_quad_vertices[] = {
        Vector3(-1, -1, 1), Vector3(1, -1, 1), Vector3(-1, 1, 1),
        Vector3(1, 1, 1)};

    constexpr uint32_t screen_quad_indices[] = {0, 1, 2, 1, 3, 2};

    quad_vertices = Graphics::BufferBuilder(sizeof(screen_quad_vertices))
                        .isVertexBuffer(sizeof(Vector3))
                        .isCPUWritable()
                        .create()
                        .getResult();
    auto vertex_data = quad_vertices.map();
    memcpy(vertex_data, screen_quad_vertices, sizeof(screen_quad_vertices));
    quad_vertices.unmap();

    quad_indices = Graphics::BufferBuilder(sizeof(screen_quad_indices))
                       .isIndexBuffer()
                       .isCPUWritable()
                       .create()
                       .getResult();
    auto index_data = quad_indices.map();
    memcpy(index_data, screen_quad_indices, sizeof(screen_quad_indices));
    quad_indices.unmap();

    auto vertex_shader =
        Graphics::ShaderBuilder("./Assets/Shaders/Stars/Stars.spv",
                                "vertex_main", VK_SHADER_STAGE_VERTEX_BIT)
            .withConstants(sizeof(VkDeviceAddress))
            .create()
            .getResult();
    auto pixel_shader =
        Graphics::ShaderBuilder("./Assets/Shaders/Stars/Stars.spv",
                                "pixel_main", VK_SHADER_STAGE_FRAGMENT_BIT)
            .create()
            .getResult();

    auto input_layout = Graphics::InputLayoutBuilder()
                            .addElement(Graphics::InputElementFormat::Vector3f)
                            .create();

    pipeline = Graphics::GraphicsPipelineBuilder(input_layout, vertex_shader,
                                                 pixel_shader)
                   .create();

    // star_parameters_buffer =
    //     Graphics::BufferBuilder(sizeof(StarData::StarParameters))
    //         .isConstantBuffer()
    //         .isCPUWritable()
    //         .create()
    //         .getResult();
}

void StarRenderer::render(const CommandBuffer& command_buffer,
                          const Buffer& camera_data) {
    command_buffer.setPipeline(pipeline);

    VkDeviceAddress address = camera_data.getDeviceAddress();
    command_buffer.pushConstants(pipeline, VK_SHADER_STAGE_VERTEX_BIT, &address,
                                 sizeof(address));

    command_buffer.draw(quad_vertices, quad_indices);
}

}  // namespace Graphics