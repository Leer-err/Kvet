#pragma once

#include "CloudsRenderer.h"
#include "DescriptorSet.h"
#include "FrameData.h"
#include "StarRenderer.h"

namespace Graphics {

class RenderPass {
   public:
    RenderPass(DescriptorSet& descriptors);

    void render(const FrameData& frame_data);

   private:
    void beginPass(const FrameData& frame_data);
    void endPass(const FrameData& frame_data);

    Buffer camera_data;

    StarRenderer star_renderer;
    CloudsRenderer clouds_renderer;

    DescriptorSet descriptors;
};

}  // namespace Graphics