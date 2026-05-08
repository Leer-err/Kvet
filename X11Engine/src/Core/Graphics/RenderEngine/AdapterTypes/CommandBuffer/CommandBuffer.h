#pragma once

#include <vulkan/vulkan.h>

#include "Buffer.h"
#include "DescriptorSet.h"
#include "GraphicsPipeline.h"
#include "Image.h"
#include "RenderEnviroment.h"

namespace Graphics {

struct CommandBuffer {
    void begin() const;
    void end() const;

    void draw(const Buffer& vertex_buffer, const Buffer& index_buffer) const;

    void copy(Image& src, Image& dst) const;

    template <typename T>
    void pushConstants(const GraphicsPipeline& pipeline,
                       const T* constants) const {
        pushConstants(pipeline, constants, sizeof(T));
    }
    void pushConstants(const GraphicsPipeline& pipeline, const void* constants,
                       size_t constant_size) const;

    void bindDescriptorSet(const GraphicsPipeline& pipeline,
                           const DescriptorSet& set) const;
    void setPipeline(const GraphicsPipeline& pipeline) const;

    void bindRenderEnviroment(const RenderEnviroment& env) const;
    void unbindRenderEnviroment() const;

    void barrier(const VkImageMemoryBarrier2* barriers,
                 size_t barrier_count) const;

    void reset() const;

    VkCommandBufferSubmitInfo submit() const;

    VkCommandBuffer buffer;
};

}  // namespace Graphics