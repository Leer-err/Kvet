#pragma once

#include "Buffer.h"
#include "FrameData.h"
#include "GraphicsPipeline.h"
#include "StarsData.h"

namespace Graphics {

class StarRenderer {
   public:
    StarRenderer(const DescriptorSet& descriptors);

    void render(const FrameData& frame_data, const Buffer& camera_data,
                const StarsData& stars_data);

   private:
    Graphics::Buffer quad_vertices;
    Graphics::Buffer quad_indices;

    Graphics::GraphicsPipeline pipeline;

    Graphics::Buffer stars_data_buffer;
};

}  // namespace Graphics