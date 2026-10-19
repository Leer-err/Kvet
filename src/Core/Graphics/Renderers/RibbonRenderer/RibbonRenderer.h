#pragma once

#include <vector>

#include "Device.h"
#include "EngineData.h"
#include "FrameGraph.h"
#include "Handles.h"
#include "Particle.h"
#include "RenderWorld.h"

namespace Graphics {

class RibbonRenderer {
   public:
    RibbonRenderer(Device& device, const EngineData& engine_data,
                   BufferHandle camera_buffer);

    void render(FrameGraph& frame_graph, RenderWorld& world);

   private:
    size_t generateMeshlets(const std::vector<Ribbon>& ribbons);

    static BufferHandle createNodeBuffer(Device& device);
    static BufferHandle createMeshletIndicesBuffer(Device& device);
    static BufferHandle createMeshletsBuffer(Device& device);

    BufferHandle camera_buffer;

    EngineData engine_data;

    BufferHandle nodes;
    BufferHandle meshlet_indices;
    BufferHandle meshlets;
};

}  // namespace Graphics