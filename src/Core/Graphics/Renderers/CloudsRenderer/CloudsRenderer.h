#pragma once

#include <vulkan/vulkan.h>

#include "Buffer.h"
#include "Device.h"
#include "EngineData.h"
#include "FrameGraph.h"
#include "GraphicsPipeline.h"
#include "Handles.h"
#include "RenderWorld.h"

namespace Graphics {

class CloudsRenderer {
    struct PushConstants {
        VkDeviceAddress camera_address;
        VkDeviceAddress clouds_address;
    };

    struct Parameters {
        float time;

        Vector3 color;
        float height;
        float cloud_plane_scale;
    };

   public:
    CloudsRenderer(Device& device, const EngineData& engine_data);

    void render(FrameGraph& frame_graph, const RenderWorld& world);

    void setCameraData(VkDeviceAddress camera_data);

   private:
    static BufferHandle createCloudDataBuffer(Device& device);
    static Mesh createScreenQuad(Device& device, const EngineData& engine_data);
    static Mesh createCloudPlane(Device& device, const EngineData& engine_data);

    EngineData engine_data;

    Mesh quad;
    Mesh cloud_plane;

    GraphicsPipeline cloud_texture_pipeline;
    GraphicsPipeline cloud_pipeline;

    TextureHandle clouds_texture;
    BufferHandle clouds_data_buffer;

    PushConstants push_constants;
};

}  // namespace Graphics