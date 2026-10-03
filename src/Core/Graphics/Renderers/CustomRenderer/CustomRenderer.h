#pragma once

#include "EngineData.h"
#include "FrameGraph.h"
#include "Handles.h"
#include "RenderWorld.h"

namespace Graphics {

class CustomRenderer {
   public:
    CustomRenderer(Device& device, const EngineData& engine_data,
                   BufferHandle camera_buffer);

    void render(FrameGraph& frame_graph, RenderWorld& world);

   private:
    BufferHandle camera_buffer;

    EngineData engine_data;
};

}  // namespace Graphics