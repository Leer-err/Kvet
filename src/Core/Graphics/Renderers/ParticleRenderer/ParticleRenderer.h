#pragma once

#include <vulkan/vulkan.h>

#include "EngineData.h"
#include "FrameGraph.h"
#include "Graphics.h"
#include "RenderWorld.h"

namespace Graphics {

class ParticleRenderer {
    static constexpr size_t MAX_PARTICLE_COUNT = 10000;

    struct PushConstants {
        VkDeviceAddress camera_data;
        VkDeviceAddress particles_data;
        VkDeviceAddress live_particles_data;
    };

   public:
    ParticleRenderer(Device& device, const EngineData& engine_data);

    void render(FrameGraph& frame_graph, const RenderWorld& world);

    void setCameraData(VkDeviceAddress camera_data);

   private:
    static MeshHandle createQuadMesh(const EngineData& engine_data);
    static BufferHandle createParticleBuffer(Device& device);
    static BufferHandle createLiveParticleBuffer(Device& device);

    EngineData engine_data;

    BufferHandle particle_buffer;
    BufferHandle live_particles_buffer;

    GraphicsPipeline pipeline;
    PushConstants push_constants;
    MeshHandle quad;
};

}  // namespace Graphics