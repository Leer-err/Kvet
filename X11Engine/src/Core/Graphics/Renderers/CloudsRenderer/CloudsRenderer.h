#pragma once

#include "Buffer.h"
#include "CloudsData.h"
#include "CommandBuffer.h"
#include "DescriptorSet.h"
#include "GraphicsPipeline.h"
#include "RenderEnviroment.h"

namespace Graphics {

class CloudsRenderer {
   public:
    CloudsRenderer(const DescriptorSet& descriptors);

    void render(const CommandBuffer& command_buffer, const Buffer& camera_data,
                const CloudsData& clouds_data);

    void preRender(const CommandBuffer& command_buffer,
                   const Buffer& camera_data, const CloudsData& clouds_data);

   private:
    Buffer quad_vertices;
    Buffer quad_indices;

    GraphicsPipeline pipeline;
    RenderEnviroment env;

    Image clouds_texture;
    Buffer clouds_data_buffer;
};

}  // namespace Graphics