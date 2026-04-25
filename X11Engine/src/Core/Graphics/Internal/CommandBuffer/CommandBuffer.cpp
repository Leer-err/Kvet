#include "CommandBuffer.h"

#include <vulkan/vulkan_core.h>

#include <cstddef>

#include "Buffer.h"
#include "GraphicsPipeline.h"
#include "RenderEnviroment.h"

namespace Graphics {

void CommandBuffer::begin() const {
    VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(buffer, &info);
}

void CommandBuffer::copy(Image& src, Image& dst) const {
    VkImageCopy2 copy_region = {};
    copy_region.sType = VK_STRUCTURE_TYPE_IMAGE_COPY_2;
    copy_region.extent.width = src.width;
    copy_region.extent.height = src.height;
    copy_region.extent.depth = 1;
    copy_region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy_region.srcSubresource.layerCount = 1;
    copy_region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy_region.dstSubresource.layerCount = 1;

    VkCopyImageInfo2 copy_info = {};
    copy_info.sType = VK_STRUCTURE_TYPE_COPY_IMAGE_INFO_2;
    copy_info.srcImage = src.image;
    copy_info.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    copy_info.dstImage = dst.image;
    copy_info.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    copy_info.regionCount = 1;
    copy_info.pRegions = &copy_region;

    vkCmdCopyImage2(buffer, &copy_info);
}

void CommandBuffer::pushConstants(const GraphicsPipeline& pipeline,
                                  VkShaderStageFlagBits stage,
                                  const void* constants,
                                  size_t constant_size) const {
    vkCmdPushConstants(buffer, pipeline.layout, stage, 0, constant_size,
                       constants);
}

void CommandBuffer::setPipeline(const GraphicsPipeline& pipeline) const {
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      pipeline.pipeline);
}

void CommandBuffer::draw(const Buffer& vertex_buffer,
                         const Buffer& index_buffer) const {
    vkCmdBindIndexBuffer(buffer, index_buffer.buffer, 0, VK_INDEX_TYPE_UINT32);

    VkDeviceSize vertex_buffer_offset = 0;
    vkCmdBindVertexBuffers(buffer, 0, 1, &vertex_buffer.buffer,
                           &vertex_buffer_offset);

    auto index_count = index_buffer.getSize() / sizeof(uint32_t);
    vkCmdDrawIndexed(buffer, index_count, 3, 0, 0, 0);
}

void CommandBuffer::bindRenderEnviroment(const RenderEnviroment& env) const {
    VkRenderingInfo render_info = {};
    render_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    render_info.renderArea.extent.width = env.render_target.width;
    render_info.renderArea.extent.height = env.render_target.height;
    render_info.layerCount = 1;

    VkRenderingAttachmentInfo colorAttachmentInfo = {};
    colorAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    colorAttachmentInfo.imageView = env.render_target.render_target;
    colorAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    colorAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    if (env.clear_render_target) {
        colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachmentInfo.clearValue.color =
            env.render_target_clear_value.color;
    }

    render_info.colorAttachmentCount = 1;
    render_info.pColorAttachments = &colorAttachmentInfo;

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

    vkCmdBeginRendering(buffer, &render_info);

    VkViewport viewport = {};
    viewport.width = static_cast<float>(env.render_target.width);
    viewport.height = static_cast<float>(env.render_target.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(buffer, 0, 1, &viewport);

    VkRect2D scissor = {};
    scissor.extent.width = static_cast<uint32_t>(env.render_target.width);
    scissor.extent.height = static_cast<uint32_t>(env.render_target.height);
    vkCmdSetScissor(buffer, 0, 1, &scissor);
}

void CommandBuffer::unbindRenderEnviroment() const {
    vkCmdEndRendering(buffer);
}

void CommandBuffer::end() const { vkEndCommandBuffer(buffer); }

void CommandBuffer::reset() const { vkResetCommandBuffer(buffer, 0); }

}  // namespace Graphics