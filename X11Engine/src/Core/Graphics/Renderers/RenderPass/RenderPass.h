#pragma once

#include "CloudsRenderer.h"
#include "CommandBuffer.h"
#include "DescriptorSet.h"
#include "RenderEnviroment.h"
#include "StarRenderer.h"

namespace Graphics {

class RenderPass {
   public:
    RenderPass(DescriptorSet& descriptors);

    void render(CommandBuffer& cmd, const RenderEnviroment& env);

   private:
    void beginPass(const CommandBuffer& cmd, const RenderEnviroment& env);
    void endPass(const CommandBuffer& cmd);

    Buffer camera_data;

    StarRenderer star_renderer;
    CloudsRenderer clouds_renderer;

    DescriptorSet descriptors;
};

}  // namespace Graphics