#include "CustomRenderer.h"

#include "FrameGraph.h"
#include "Graphics.h"

namespace Graphics {

void CustomRenderer::render(FrameGraph& frame_graph, const RenderWorld& world) {
    CustomRenderObject ro;

    auto pass =
        GraphicsPass("", ro.getPipeline(),
                     [this, &world, &ro](GraphicsPassExecution& execution) {
                         execution.appendData(ro.getData());

                         execution.draw(ro.getMesh());
                     });

    pass.addColorAttachment();
    pass.setDepthAttachment();

    frame_graph.addGraphicsPass(pass);
}

}  // namespace Graphics