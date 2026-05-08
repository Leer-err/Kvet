#include "CloudsRenderer.h"

#include <vulkan/vulkan.h>

#include <cstring>

#include "BufferBuilder.h"
#include "CloudsData.h"
#include "DescriptorSet.h"
#include "GraphicsPipelineBuilder.h"
#include "ImageBuilder.h"
#include "InputLayoutBuilder.h"
#include "Sampler.h"
#include "ShaderBuilder.h"
#include "Vector2.h"
#include "Vector3.h"

namespace Graphics {

struct Vertex {
    Vector3 vertex_position;
    Vector2 uv;
};

CloudsRenderer::CloudsRenderer(const EngineData& engine_data) {
    constexpr Vertex cloud_plane_vertex_data[] = {
        {Vector3(-1, 0, -1), Vector2(0, 0)},
        {Vector3(-1, 0, 1), Vector2(0, 1)},
        {Vector3(1, 0, -1), Vector2(1, 0)},
        {Vector3(1, 0, 1), Vector2(1, 1)}};

    constexpr Vector3 screen_quad_vertices[] = {
        Vector3(-1, -1, 1), Vector3(1, -1, 1), Vector3(-1, 1, 1),
        Vector3(1, 1, 1)};

    constexpr uint32_t screen_quad_indices[] = {0, 1, 2, 1, 3, 2};

    cloud_plane_vertices =
        BufferBuilder(engine_data, sizeof(cloud_plane_vertex_data))
            .isVertexBuffer(sizeof(Vector3))
            .isCPUWritable()
            .create()
            .getResult();
    auto cloud_plane_data = engine_data.device.map(cloud_plane_vertices);
    memcpy(cloud_plane_data, cloud_plane_vertex_data,
           sizeof(cloud_plane_vertex_data));
    engine_data.device.unmap(cloud_plane_vertices);

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

    clouds_texture =
        ImageBuilder(engine_data, VK_FORMAT_R8G8B8A8_UNORM, 512, 512)
            .isRenderTarget()
            .isShaderResource()
            .create()
            .getResult();
    engine_data.descriptor_set.addImage(
        engine_data.device.createRenderTarget(clouds_texture));
    engine_data.descriptor_set.addSampler(Sampler::point(engine_data));

    env.width = 512;
    env.height = 512;
    env.render_target = engine_data.device.createRenderTarget(clouds_texture);
    env.clear_render_target = true;
    env.render_target_clear_value.color = {0, 0, 0, 0};

    cloud_texture_pipeline =
        GraphicsPipelineBuilder(
            engine_data, "./Assets/Shaders/Clouds/CloudsTexture.spv",
            "vertex_main", "./Assets/Shaders/Clouds/CloudsTexture.spv",
            "pixel_main")
            .setRenderTargetFormat(VK_FORMAT_R8G8B8A8_UNORM)
            .create()
            .getResult();
    cloud_pipeline =
        GraphicsPipelineBuilder(
            engine_data, "./Assets/Shaders/Clouds/Clouds.spv", "vertex_main",
            "./Assets/Shaders/Clouds/Clouds.spv", "pixel_main")
            .create()
            .getResult();

    clouds_data_buffer = BufferBuilder(engine_data, sizeof(CloudsData))
                             .isConstantBuffer()
                             .isCPUWritable()
                             .create()
                             .getResult();
    push_constants.clouds_address = clouds_data_buffer.device_address;

    CloudsData data = {};
    data.color = Vector3(1, 0, 1);
    data.time = 0;
    data.cloud_plane_scale = 1000;
    data.height = 100;

    auto data_ptr = engine_data.device.map(clouds_data_buffer);
    memcpy(data_ptr, &data, sizeof(CloudsData));
    engine_data.device.unmap(clouds_data_buffer);
}

void CloudsRenderer::render(const FrameData& frame_data,
                            const CloudsData& clouds_data) {
    TracyVkZone(frame_data.trace_ctx, frame_data.cmd.buffer, "Clouds");

    auto command_buffer = frame_data.cmd;

    command_buffer.setPipeline(cloud_pipeline);
    command_buffer.bindDescriptorSet(cloud_pipeline, frame_data.descriptor_set);

    command_buffer.pushConstants(cloud_pipeline, &push_constants);

    command_buffer.draw(cloud_plane_vertices, quad_indices);
}

void CloudsRenderer::preRender(const FrameData& frame_data,
                               const CloudsData& clouds_data) {
    TracyVkZone(frame_data.trace_ctx, frame_data.cmd.buffer,
                "Cloud texture bake");

    auto command_buffer = frame_data.cmd;

    auto render_target_barrier = clouds_texture.createBarrier(
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_PIPELINE_STAGE_2_NONE,
        VK_ACCESS_2_NONE, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT);

    command_buffer.barrier(&render_target_barrier, 1);

    command_buffer.bindRenderEnviroment(env);

    command_buffer.setPipeline(cloud_texture_pipeline);
    command_buffer.bindDescriptorSet(cloud_texture_pipeline,
                                     frame_data.descriptor_set);

    command_buffer.pushConstants(cloud_texture_pipeline,
                                 &clouds_data_buffer.device_address);

    command_buffer.draw(quad_vertices, quad_indices);

    command_buffer.unbindRenderEnviroment();

    auto cloud_texture_barrier = clouds_texture.createBarrier(
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT, VK_ACCESS_2_SHADER_READ_BIT);
    command_buffer.barrier(&cloud_texture_barrier, 1);
}

void CloudsRenderer::setCameraData(VkDeviceAddress camera_data) {
    push_constants.camera_address = camera_data;
}

}  // namespace Graphics