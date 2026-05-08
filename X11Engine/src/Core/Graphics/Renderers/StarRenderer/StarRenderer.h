#pragma once

#include "Buffer.h"
#include "EngineData.h"
#include "FrameData.h"
#include "GraphicsPipeline.h"
#include "StarsData.h"

namespace Graphics {

class StarRenderer {
    struct PushConstants {
        VkDeviceAddress camera_data;
        VkDeviceAddress stars_data;
    };

   public:
    StarRenderer(const EngineData& engine_data);

    void render(const FrameData& frame_data, const Buffer& camera_data,
                const StarsData& stars_data);

    void setCameraData(VkDeviceAddress camera_data);

   private:
    EngineData engine_data;

    Buffer quad_vertices;
    Buffer quad_indices;

    GraphicsPipeline pipeline;

    Buffer stars_data_buffer;

    PushConstants push_constants;
};

}  // namespace Graphics