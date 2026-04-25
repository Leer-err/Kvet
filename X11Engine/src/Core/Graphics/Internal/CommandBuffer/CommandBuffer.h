#pragma once

#include <vulkan/vulkan_core.h>

#include "Buffer.h"
#include "GraphicsPipeline.h"
#include "Image.h"
#include "RenderEnviroment.h"

namespace Graphics {

struct CommandBuffer {
    void begin() const;
    void end() const;

    void draw(const Buffer& vertex_buffer, const Buffer& index_buffer) const;

    void copy(Image& src, Image& dst) const;

    void pushConstants(const GraphicsPipeline& pipeline,
                       VkShaderStageFlagBits stage, const void* constants,
                       size_t constant_size) const;

    void setPipeline(const GraphicsPipeline& pipeline) const;

    void bindRenderEnviroment(const RenderEnviroment& env) const;
    void unbindRenderEnviroment() const;

    void reset() const;

    VkCommandBuffer buffer;
};

}  // namespace Graphics