#pragma once

#include <span>
#include <vector>

#include "CameraData.h"
#include "CloudsData.h"
#include "EffectDescription.h"
#include "Graphics.h"
#include "PostProcessingData.h"
#include "RenderObjectData.h"
#include "StarsData.h"
#include "VFXWorld.h"

namespace Graphics {

using OpaqueObjectHandle = uint32_t;

class RenderWorld : public IRenderWorld {
   public:
    RenderWorld();

    RenderData& renderData() override;
    const RenderData& renderData() const override;

    void addRenderObject(const RenderObjectData& data) override;
    RenderObjectData& getOpaqueObject(const OpaqueObjectHandle& handle);
    std::span<const RenderObjectData> getOpaqueObjects() const;

    void addEffect(const EffectDescription& description) override;
    void addMeshEffect(const MeshEffectDescription& description);

    void update(float delta_time);

    VFXWorld::ParticleBatch getParticles() const;
    VFXWorld::MeshParticleBatch getMeshParticles() const;

   private:
    std::vector<RenderObjectData> opaque_objects;
    OpaqueObjectHandle next_handle;

    VFXWorld vfx_world;

    RenderData render_data;
};

}  // namespace Graphics