#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

#include "EffectDescription.h"
#include "Filesystem.h"
#include "Handle.h"
#include "Matrix.h"
#include "RenderObjectData.h"
#include "Vector3.h"

namespace Graphics {

class Texture;
class Mesh;

using TextureHandle = Handle<Texture>;
using MeshHandle = Handle<Mesh>;
using GraphicsPipelineHandle = Handle<GraphicsPipeline>;

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

struct CustomRenderObjectData {};

class CustomRenderObject {
    static constexpr size_t MAX_TEXTURES_PER_MATERIAL = 8;

   public:
    void updateData(const uint8_t* data, size_t size);
    void updateModelMatrix(const Matrix& model);

    std::span<TextureHandle> getTextures() const;
    std::span<uint8_t> getData() const;
    MeshHandle getMesh() const;
    Matrix getModelMatrix() const;
    GraphicsPipelineHandle getPipeline() const;

   private:
    Matrix model;
    GraphicsPipelineHandle pipeline;
    MeshHandle mesh;

    std::array<TextureHandle, MAX_TEXTURES_PER_MATERIAL> textures;
    std::vector<uint8_t> data;
};

class IResourceManager {
   public:
    virtual std::optional<TextureHandle> addTexture(
        const File::Texture& texture) = 0;
    virtual std::optional<TextureHandle> getTexture(std::string_view name) = 0;

    virtual std::optional<MeshHandle> addMesh(const File::Mesh& mesh) = 0;
    virtual std::optional<MeshHandle> getMesh(std::string_view name) = 0;

    virtual std::optional<GraphicsPipelineHandle> addPipeline(
        const File::Pipeline& shader) = 0;
    virtual std::optional<GraphicsPipelineHandle> getPipeline(
        std::string_view name) = 0;
};

class IRenderWorld {
   public:
    virtual RenderData& renderData() = 0;
    virtual const RenderData& renderData() const = 0;

    virtual void addRenderObject(const RenderObjectData& data) = 0;
    virtual Handle<CustomRenderObject> addCustomRenderObject(
        MeshHandle mesh, GraphicsPipelineHandle handle, size_t data_size) = 0;

    virtual void addEffect(const EffectDescription& data) = 0;
    virtual void addMeshEffect(const MeshEffectDescription& data) = 0;
};

class IRenderEngine {
   public:
    virtual void render() = 0;

    virtual const IRenderWorld* getRenderWorld() const = 0;
    virtual IRenderWorld* getRenderWorld() = 0;
    virtual const IResourceManager* getResourceManager() const = 0;
    virtual IResourceManager* getResourceManager() = 0;
};

bool init();

IRenderEngine* getRenderEngine();

}  // namespace Graphics