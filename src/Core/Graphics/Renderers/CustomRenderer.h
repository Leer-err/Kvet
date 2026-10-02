#pragma once

#include "FrameGraph.h"
#include "RenderWorld.h"

namespace Graphics {

class CustomRenderer {
   public:
    void render(FrameGraph& frame_graph, const RenderWorld& world);

   private:
};

}  // namespace Graphics