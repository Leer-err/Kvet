#pragma once

#include <span>
#include <vector>

#include "EffectDescription.h"
#include "Graphics.h"
#include "Handles.h"
#include "Matrix.h"
#include "PoolAllocator.h"
#include "RenderObjectData.h"
#include "ResourceRegistry.h"
#include "VFXWorld.h"

namespace Graphics {

struct CustomRenderObjectData {
    CustomRenderObjectData(const CustomRenderObjectCreateData& data)
        : model(data.model), pipeline(data.pipeline), mesh(data.mesh) {}

    Matrix model;
    GraphicsPipelineHandle pipeline;
    MeshHandle mesh;
};

class RenderWorld : public IRenderWorld {
   public:
    RenderWorld();

    RenderData& renderData() override;
    const RenderData& renderData() const override;

    void addRenderObject(const RenderObjectData& data) override;
    std::span<const RenderObjectData> getOpaqueObjects() const;
    CustomRenderObject addCustomRenderObject(
        const CustomRenderObjectCreateData& data) override;
    std::vector<Handle<CustomRenderObjectData>> getCustomRenderObjects();

    void addEffect(const EffectDescription& description) override;
    void addMeshEffect(const MeshEffectDescription& description) override;

    void update();

    VFXWorld::ParticleBatch getParticles() const;
    VFXWorld::MeshParticleBatch getMeshParticles() const;

   private:
    std::vector<RenderObjectData> opaque_objects;

    PoolAllocator custom_ro_allocator;
    ResourceRegistry<CustomRenderObjectData> custom_ro_registry;

    VFXWorld vfx_world;

    RenderData render_data;
};

}  // namespace Graphics