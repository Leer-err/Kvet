#pragma once

#include "EngineData.h"
#include "FrameData.h"

namespace Graphics {

class OverlayRenderer {
   public:
    OverlayRenderer(const EngineData& engine_data);

    void render(const FrameData& frame_data);

   private:
};

}  // namespace Graphics