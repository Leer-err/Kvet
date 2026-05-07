#pragma once

#include "APIData.h"
#include "Buffer.h"
#include "EngineData.h"
#include "FrameData.h"
#include "GraphicsPipeline.h"
#include "StarsData.h"

namespace Graphics {

class StarRenderer {
   public:
    StarRenderer(const APIData& api_data, const EngineData& engine_data);

    void render(const FrameData& frame_data, const Buffer& camera_data,
                const StarsData& stars_data);

   private:
    Graphics::Buffer quad_vertices;
    Graphics::Buffer quad_indices;

    Graphics::GraphicsPipeline pipeline;

    Graphics::Buffer stars_data_buffer;
};

}  // namespace Graphics