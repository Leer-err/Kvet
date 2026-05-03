#include "CloudsRenderer.h"

#include <vulkan/vulkan.h>

#include <cstring>

#include "BufferBuilder.h"
#include "CloudsData.h"
#include "GraphicsPipelineBuilder.h"
#include "ImageBuilder.h"
#include "InputLayoutBuilder.h"
#include "ShaderBuilder.h"
#include "Vector3.h"

namespace Graphics {

CloudsRenderer::CloudsRenderer(const DescriptorSet& descriptors) {
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

    clouds_texture = ImageBuilder(VK_FORMAT_R8G8B8A8_UNORM, 512, 512)
                         .isRenderTarget()
                         .isShaderResource()
                         .create()
                         .getResult();

    env.render_target = RenderTarget::create(clouds_texture);
    env.clear_render_target = true;
    env.render_target_clear_value.color = {0, 0, 0, 0};

    pipeline = GraphicsPipelineBuilder(
                   "./Assets/Shaders/Clouds/CloudsTexture.spv", "vertex_main",
                   "./Assets/Shaders/Clouds/CloudsTexture.spv", "pixel_main",
                   descriptors)
                   .create()
                   .getResult();

    clouds_data_buffer = BufferBuilder(sizeof(CloudsData))
                             .isConstantBuffer()
                             .isCPUWritable()
                             .create()
                             .getResult();

    CloudsData data = {};
    data.color = Vector3(1, 0, 1);
    data.time = 0;

    auto data_ptr = clouds_data_buffer.map();
    memcpy(data_ptr, &data, sizeof(CloudsData));
    clouds_data_buffer.unmap();
}

void CloudsRenderer::render(const CommandBuffer& command_buffer,
                            const Buffer& camera_data,
                            const CloudsData& clouds_data) {
    command_buffer.setPipeline(pipeline);

    VkDeviceAddress clouds_address = clouds_data_buffer.getDeviceAddress();
    command_buffer.pushConstants(pipeline, &clouds_address,
                                 sizeof(clouds_address));

    command_buffer.draw(quad_vertices, quad_indices);
}

void CloudsRenderer::preRender(const CommandBuffer& command_buffer,
                               const Buffer& camera_data,
                               const CloudsData& clouds_data) {
    auto render_target_barrier = clouds_texture.createBarrier(
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_PIPELINE_STAGE_2_NONE,
        VK_ACCESS_2_NONE, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT);

    command_buffer.barrier(&render_target_barrier, 1);

    command_buffer.bindRenderEnviroment(env);

    command_buffer.setPipeline(pipeline);

    VkDeviceAddress stars_address = clouds_data_buffer.getDeviceAddress();
    command_buffer.pushConstants(pipeline, &stars_address,
                                 sizeof(stars_address));

    command_buffer.draw(quad_vertices, quad_indices);

    command_buffer.unbindRenderEnviroment();
}

}  // namespace Graphics