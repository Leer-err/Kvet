#pragma once

#include "CloudsRenderer.h"
#include "DescriptorSet.h"
#include "FrameData.h"
#include "OverlayRenderer.h"
#include "StarRenderer.h"

namespace Graphics {

class RenderPass {
   public:
    RenderPass(EngineData engine_data);

    void render(const FrameData& frame_data);

   private:
    void beginPass(const FrameData& frame_data);
    void endPass(const FrameData& frame_data);

    void updateCameraBuffer();

    EngineData engine_data;

    Buffer camera_data_buffer;

    StarRenderer star_renderer;
    CloudsRenderer clouds_renderer;
    OverlayRenderer overlay_renderer;
};

}  // namespace Graphics