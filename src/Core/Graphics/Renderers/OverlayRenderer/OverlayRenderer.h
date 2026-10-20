#pragma once

#include "EngineData.h"
#include "FrameData.h"
#include "FrameGraph.h"

namespace Graphics {

class OverlayRenderer {
   public:
    explicit OverlayRenderer(const EngineData& engine_data);

    void render(FrameGraph& frame_data);

   private:
    EngineData engine_data;
};

}  // namespace Graphics