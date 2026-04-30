#pragma once

#include "CommandBuffer.h"
#include "CommandPool.h"
#include "RenderEnviroment.h"
#include "StarRenderer.h"

namespace Graphics {

class RenderPass {
   public:
    RenderPass();

    void render(CommandBuffer& cmd, const RenderEnviroment& env);

   private:
    void beginPass(const CommandBuffer& cmd, const RenderEnviroment& env);
    void endPass(const CommandBuffer& cmd);

    Buffer camera_data;

    StarRenderer star_renderer;
};

}  // namespace Graphics