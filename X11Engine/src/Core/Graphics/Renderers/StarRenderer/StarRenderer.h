#pragma once

#include "Buffer.h"
#include "CameraData.h"
#include "CommandBuffer.h"
#include "GraphicsPipeline.h"

namespace Graphics {

class StarRenderer {
   public:
    StarRenderer();

    void render(const CommandBuffer& command_buffer, const Buffer& camera_data);

   private:
    Graphics::Buffer quad_vertices;
    Graphics::Buffer quad_indices;

    Graphics::GraphicsPipeline pipeline;

    // Graphics::Buffer star_parameters_buffer;
};

}  // namespace Graphics