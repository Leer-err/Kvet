#pragma once

#include "Buffer.h"
#include "Descriptors.h"
#include "EngineData.h"
#include "FrameData.h"
#include "FrameGraph.h"
#include "Graphics.h"
#include "PostProcessingData.h"
#include "RenderWorld.h"

namespace Graphics {

class PostProcessingPass {
    struct PostProcessingShaderData {
        std::array<uint32_t, 2> camera_dimensions;
        TextureDescriptor render_target;
        SamplerDescriptor sampler_index;

        // dithering
        float spread;

        uint32_t color_count;
    };

   public:
    PostProcessingPass(Device& device, const EngineData& engine_data);

    void render(TextureHandle input_image, FrameGraph& frame_graph,
                const RenderWorld& world);

   private:
    static MeshHandle createSqreenQuad(Device& device,
                                       const EngineData& engine_data);
    static BufferHandle createDataBuffer(Device& device);

    EngineData engine_data;

    GraphicsPipeline pipeline;
    MeshHandle quad;

    BufferHandle dithering_data_buffer;
    PostProcessingData data;
};

}  // namespace Graphics