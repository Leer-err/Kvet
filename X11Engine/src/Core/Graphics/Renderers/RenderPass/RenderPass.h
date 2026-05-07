#pragma once

#include "APIData.h"
#include "CloudsRenderer.h"
#include "DescriptorSet.h"
#include "FrameData.h"
#include "StarRenderer.h"

namespace Graphics {

class RenderPass {
   public:
    RenderPass(const APIData& api_data, const EngineData& engine_data);

    void render(const FrameData& frame_data);

   private:
    void beginPass(const FrameData& frame_data);
    void endPass(const FrameData& frame_data);

    Buffer camera_data;

    StarRenderer star_renderer;
    CloudsRenderer clouds_renderer;
};

}  // namespace Graphics