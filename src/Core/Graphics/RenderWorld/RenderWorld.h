#pragma once

#include <span>
#include <vector>

#include "CameraData.h"
#include "CloudsData.h"
#include "EffectDescription.h"
#include "PostProcessingData.h"
#include "RenderObjectData.h"
#include "StarsData.h"
#include "VFXWorld.h"

namespace Graphics {

using OpaqueObjectHandle = uint32_t;

struct RenderData {
    struct Camera {
        Matrix view_projection;
        Matrix view_projection_camera_centered;
        Vector3 up;
        Vector3 right;
    };

    struct Stars {
        float star_density;
        float blinking_speed;
        float blinking_strength;
    };

    struct Clouds {
        Vector3 color;
        float time;
        float height;
        float cloud_plane_scale;
    };

    struct PostProcessing {
        float dithering_spread;

        uint32_t channel_color_count;
    };

    float time = 0;
    float delta_time;

    Camera camera;
    Stars stars;
    Clouds clodus;
    PostProcessing post_processing;
};

class RenderWorld {
   public:
    RenderWorld();

    RenderData& renderData();
    const RenderData& renderData() const;

    OpaqueObjectHandle addOpaqueObject(const RenderObjectData& data);
    RenderObjectData& getOpaqueObject(const OpaqueObjectHandle& handle);
    std::span<const RenderObjectData> getOpaqueObjects() const;

    void addEffect(const EffectDescription& description);
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