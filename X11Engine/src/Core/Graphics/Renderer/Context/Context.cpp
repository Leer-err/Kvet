#include "Context.h"

#include <vulkan/vulkan_core.h>

#include <memory>

#include "CommandBuffer.h"
#include "GraphicsPipeline.h"
#include "GraphicsResources.h"
#include "InternalBuffer.h"
#include "InternalRenderTarget.h"
#include "InternalTexture.h"
#include "Pipeline.h"

// #include "Sampler.h"
#include "RenderTarget.h"
#include "Texture.h"
#include "Window.h"

namespace Graphics {

Context::Context() {
    auto frame = Resources::get().getFrameInFlight();
    command_buffer = std::make_unique<Internal::CommandBuffer>(frame.buffer);
}

Context::~Context() = default;

void Context::bindRenderEnviroment(const RenderEnviroment& render_enviroment) {
    VkRenderingInfo render_info = {};
    render_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    render_info.renderArea.extent.width =
        static_cast<uint32_t>(Window::get().getWidth());
    render_info.renderArea.extent.height =
        static_cast<uint32_t>(Window::get().getHeight());
    render_info.layerCount = 1;

    if (render_enviroment.render_target) {
        auto render_target =
            render_enviroment.render_target->getInternal()->render_target;

        VkRenderingAttachmentInfo colorAttachmentInfo = {};
        colorAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        colorAttachmentInfo.imageView = render_target;
        colorAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
        colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        colorAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        if (render_enviroment.clear_render_target) {
            colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachmentInfo.clearValue.color.float32[0] =
                render_enviroment.render_target_clear_value.x;
            colorAttachmentInfo.clearValue.color.float32[1] =
                render_enviroment.render_target_clear_value.y;
            colorAttachmentInfo.clearValue.color.float32[2] =
                render_enviroment.render_target_clear_value.z;
            colorAttachmentInfo.clearValue.color.float32[3] =
                render_enviroment.render_target_clear_value.w;
        }

        render_info.colorAttachmentCount = 1;
        render_info.pColorAttachments = &colorAttachmentInfo;
    }

    // VkRenderingAttachmentInfo depthAttachmentInfo{
    //     .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
    //     .imageView = depthImageView,
    //     .imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
    //     .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    //     .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    //     .clearValue = {.depthStencil = {1.0f, 0}}};

    // VkRenderingInfo renderingInfo{
    //     .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
    //     .renderArea{.extent{.width = static_cast<uint32_t>(windowSize.x),
    //                         .height = static_cast<uint32_t>(windowSize.y)}},
    //     .layerCount = 1,
    //     .colorAttachmentCount = 1,
    //     .pColorAttachments = &colorAttachmentInfo,
    //     .pDepthAttachment = &depthAttachmentInfo};

    auto command_buffer_handle = command_buffer.get()->buffer;
    vkCmdBeginRendering(command_buffer_handle, &render_info);
}

void Context::unbindRenderEnviroment() {
    auto command_buffer_handle = command_buffer.get()->buffer;
    vkCmdEndRendering(command_buffer_handle);
}

void Context::setPipeline(const GraphicsPipeline& pipeline) {
    auto pipeline_internals = pipeline.getInternal();
    auto command_buffer_handle = command_buffer.get()->buffer;

    auto width = Window::get().getWidth();
    auto height = Window::get().getHeight();

    VkViewport viewport = {};
    viewport.width = static_cast<float>(width);
    viewport.height = static_cast<float>(height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(command_buffer_handle, 0, 1, &viewport);

    VkRect2D scissor = {};
    scissor.extent.width = static_cast<uint32_t>(width);
    scissor.extent.height = static_cast<uint32_t>(height);
    vkCmdSetScissor(command_buffer_handle, 0, 1, &scissor);

    vkCmdBindPipeline(command_buffer_handle, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      pipeline_internals->pipeline);
}

void Context::draw(const Buffer& vertex_buffer, const Buffer& index_buffer) {
    auto command_buffer_handle = command_buffer.get()->buffer;

    auto index_buffer_handle = index_buffer.getInternal()->buffer;
    vkCmdBindIndexBuffer(command_buffer_handle, index_buffer_handle, 0,
                         VK_INDEX_TYPE_UINT32);

    auto vertex_buffer_handle = vertex_buffer.getInternal()->buffer;
    VkDeviceSize vertex_buffer_offset = 0;
    vkCmdBindVertexBuffers(command_buffer_handle, 0, 1, &vertex_buffer_handle,
                           &vertex_buffer_offset);

    auto index_count = index_buffer.getSize() / sizeof(uint32_t);
    vkCmdDrawIndexed(command_buffer_handle, index_count, 3, 0, 0, 0);
}

RenderTarget Context::createRenderTarget(const Texture& texture) {
    auto command_buffer_handle = command_buffer.get()->buffer;

    auto texture_internal = texture.getInternal();

    VkImageViewCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = texture_internal->image;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = texture_internal->format,
    info.subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                             .levelCount = 1,
                             .layerCount = 1};

    VkImageView view;
    vkCreateImageView(Resources::get().getDevice(), &info, nullptr, &view);

    Internal::RenderTarget render_target = {};
    render_target.render_target = view;
    render_target.format = texture_internal->format;

    return RenderTarget(render_target);
}

void Context::copy(const Texture& source, const Texture& destination) {
    VkImageMemoryBarrier2 barriers[2] = {};

    barriers[0] = source.getInternal()->createBarrier(
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_NONE,
        VK_ACCESS_2_NONE, VK_PIPELINE_STAGE_2_TRANSFER_BIT,
        VK_ACCESS_2_TRANSFER_WRITE_BIT);

    barriers[1] = destination.getInternal()->createBarrier(
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_2_TRANSFER_BIT, VK_ACCESS_2_TRANSFER_READ_BIT);

    VkDependencyInfo dep = {};
    dep.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dep.imageMemoryBarrierCount = 2;
    dep.pImageMemoryBarriers = barriers;

    // vkCmdPipelineBarrier2(frame.buffer.buffer, &dep);
}

}  // namespace Graphics

// Context::Context() { context = APIResources::get().getContext(); }

// void Context::setPipeline(const GraphicsPipeline& pipeline) {
//     ID3D11RenderTargetView* render_target =
//         pipeline.getRenderTarget().get().Get();
//     ID3D11DepthStencilView* depth_stencil =
//         pipeline.getDepthStencil().get().Get();
//     context->OMSetRenderTargets(1, &render_target, depth_stencil);

//     auto viewport = pipeline.getRenderTarget().getViewport();
//     context->RSSetViewports(1, &viewport);

//     auto input_layout = pipeline.getInputLayout().get();
//     context->IASetInputLayout(input_layout.Get());

//     auto vertex_shader = pipeline.getVertexShader().get();
//     auto pixel_shader = pipeline.getPixelShader().get();
//     context->VSSetShader(vertex_shader.Get(), nullptr, 0);
//     context->PSSetShader(pixel_shader.Get(), nullptr, 0);

//     auto rasterizer = pipeline.getRasterizer().get();
//     context->RSSetState(rasterizer.Get());
// }

// void Context::cleanPipeline() {
//     context->OMSetRenderTargets(0, nullptr, nullptr);

//     ID3D11Buffer*
//         constant_buffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] =
//             {};
//     context->VSSetConstantBuffers(
//         0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT,
//         &constant_buffers[0]);
//     context->PSSetConstantBuffers(
//         0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT,
//         &constant_buffers[0]);

//     ID3D11SamplerState* samplers[D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT] =
//         {};
//     context->VSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT,
//                            &samplers[0]);
//     context->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT,
//                            &samplers[0]);

//     ID3D11ShaderResourceView*
//         resources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {};
//     context->VSSetShaderResources(
//         0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, &resources[0]);
//     context->PSSetShaderResources(
//         0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, &resources[0]);
// }

// void Context::draw(const Mesh& mesh) {
//     ID3D11Buffer* vertex_buffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] =
//     {
//         mesh.vertex_buffer.get().Get()};
//     UINT offsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {
//         static_cast<UINT>(mesh.vertex_buffer.getOffset())};
//     UINT strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {
//         static_cast<UINT>(mesh.vertex_buffer.getStride())};

//     context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//     context->IASetVertexBuffers(0, 1, vertex_buffers, strides, offsets);
//     context->IASetIndexBuffer(mesh.index_buffer.get().Get(),
//                               DXGI_FORMAT_R32_UINT,
//                               mesh.index_buffer.getOffset());

//     auto index_count =
//         static_cast<UINT>(mesh.index_buffer.getSize() / sizeof(UINT));
//     context->DrawIndexed(index_count, 0, 0);
// }

// void Context::copy(const Texture& source, Texture destination) {
//     context->CopyResource(destination.get().Get(), source.get().Get());
// }

// void* Context::internalMap(Buffer buffer) {
//     D3D11_MAPPED_SUBRESOURCE mapped_resource;

//     context->Map(buffer.get().Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL,
//                  &mapped_resource);

//     return mapped_resource.pData;
// }

// void Context::unmapConstantBuffer(const Buffer& buffer) {
//     context->Unmap(buffer.get().Get(), 0);
// }

// void Context::bindConstantBuffer(const Buffer& buffer, const Binding&
// binding) {
//     auto constant_buffer = buffer.get().Get();

//     switch (binding.shader_type) {
//         case Binding::ShaderType::Vertex:
//             context->VSSetConstantBuffers(binding.index, 1,
//             &constant_buffer); return;
//         case Binding::ShaderType::Pixel:
//             context->PSSetConstantBuffers(binding.index, 1,
//             &constant_buffer); return;
//     }
// }

// void Context::unbindConstantBuffer(const Binding& binding) {
//     bindConstantBuffer(Buffer(), binding);
// }

// void Context::bindShaderResource(const ShaderResource& resource,
//                                  const Binding& binding) {
//     auto view = resource.get().Get();

//     switch (binding.shader_type) {
//         case Binding::ShaderType::Vertex:
//             context->VSSetShaderResources(binding.index, 1, &view);
//             return;
//         case Binding::ShaderType::Pixel:
//             context->PSSetShaderResources(binding.index, 1, &view);
//             return;
//     }
// }

// void Context::unbindShaderResource(const Binding& binding) {
//     bindShaderResource(ShaderResource(), binding);
// }

// void Context::bindSampler(const Sampler& sampler, const Binding& binding) {
//     auto sampler_state = sampler.get().Get();

//     switch (binding.shader_type) {
//         case Binding::ShaderType::Vertex:
//             context->VSSetSamplers(binding.index, 1, &sampler_state);
//             return;
//         case Binding::ShaderType::Pixel:
//             context->PSSetSamplers(binding.index, 1, &sampler_state);
//             return;
//     }
// }

// void Context::unbindSampler(const Binding& binding) {
//     bindSampler(Sampler(), binding);
// }

// void Context::clean(RenderTarget render_target) {
//     constexpr FLOAT clear_color[4] = {};

//     context->ClearRenderTargetView(render_target.get().Get(), clear_color);
// }
