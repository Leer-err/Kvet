#pragma once

#include <optional>
#include <string_view>

#include "EffectDescription.h"
#include "Filesystem/Readers.h"
#include "Handle.h"
#include "Matrix.h"
#include "MeshHandle.h"
#include "RenderObjectData.h"

namespace Graphics {

class Texture;
class Mesh;

using TextureHandle = Handle<Texture>;
using MeshHandle = Handle<Mesh>;

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
        float dithering_spread = 0.03f;

        uint32_t channel_color_count = 32;
    };

    float time = 0;
    float delta_time;

    Camera camera;
    Stars stars;
    Clouds clodus;
    PostProcessing post_processing;
};

class IResourceManager {
   public:
    virtual std::optional<TextureHandle> addTexture(
        const File::TextureFile& texture) = 0;
    virtual std::optional<TextureHandle> getTexture(std::string_view name) = 0;

    virtual std::optional<MeshHandle> addMesh(
        const File::MeshFile& texture) = 0;
    virtual std::optional<MeshHandle> getMesh(std::string_view name) = 0;
};

class IRenderWorld {
   public:
    virtual RenderData& renderData() = 0;
    virtual const RenderData& renderData() const = 0;

    virtual void addRenderObject(const RenderObjectData& data) = 0;

    virtual void addEffect(const EffectDescription& data) = 0;
};

class IRenderEngine {
   public:
    virtual void render() = 0;

    virtual IRenderWorld* getRenderWorld() const = 0;
    virtual IResourceManager* getResourceManager() const = 0;
};

bool init();

IRenderEngine* getRenderEngine();

}  // namespace Graphics