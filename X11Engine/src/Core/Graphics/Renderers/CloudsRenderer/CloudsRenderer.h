#pragma once

#include "APIData.h"
#include "Buffer.h"
#include "CloudsData.h"
#include "CommandBuffer.h"
#include "DescriptorSet.h"
#include "EngineData.h"
#include "FrameData.h"
#include "GraphicsPipeline.h"
#include "RenderEnviroment.h"

namespace Graphics {

class CloudsRenderer {
   public:
    CloudsRenderer(const APIData& api_data, EngineData& engine_data);

    void render(const FrameData& frame_data, const Buffer& camera_data,
                const CloudsData& clouds_data);

    void preRender(const FrameData& frame_data, const Buffer& camera_data,
                   const CloudsData& clouds_data);

   private:
    Buffer cloud_plane_vertices;
    Buffer quad_vertices;
    Buffer quad_indices;

    GraphicsPipeline cloud_texture_pipeline;
    GraphicsPipeline cloud_pipeline;
    RenderEnviroment env;

    Image clouds_texture;
    Buffer clouds_data_buffer;
};

}  // namespace Graphics