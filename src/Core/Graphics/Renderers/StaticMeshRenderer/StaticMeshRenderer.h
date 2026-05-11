#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "EngineData.h"
#include "FrameData.h"
#include "StaticModelData.h"

namespace Graphics {

class StaticMeshRenderer {
    struct PushConstants {
        VkDeviceAddress camera_data;
        VkDeviceAddress model_data;
    };

   public:
    StaticMeshRenderer(const EngineData& engine_data);

    void render(const FrameData& frame_data, const StaticModelData& model_data);
    void setCameraData(VkDeviceAddress camera_data);

   private:
    EngineData engine_data;

    GraphicsPipeline pipeline;

    Buffer model_data_buffer;

    PushConstants push_constants;
};

}  // namespace Graphics