#include "RenderWorld.h"

#include <span>

#include "BufferBuilder.h"
#include "Device.h"
#include "Texture.h"
#include "VFXWorld.h"

namespace Graphics {

RenderWorld::RenderWorld(Device& device)
    : custom_ro_allocator(1000, sizeof(CustomRenderObjectData),
                          alignof(CustomRenderObjectData)),
      custom_ro_registry(custom_ro_allocator),
      vfx_world(10000),
      device(device) {}

RenderData& RenderWorld::renderData() { return render_data; }

const RenderData& RenderWorld::renderData() const { return render_data; }

void RenderWorld::addRenderObject(const RenderObjectData& data) {
    opaque_objects.emplace_back(data);
}

std::span<const RenderObjectData> RenderWorld::getOpaqueObjects() const {
    return opaque_objects;
}

void RenderWorld::addEffect(const EffectDescription& description) {
    vfx_world.addEffect(description);
}

void RenderWorld::addMeshEffect(const MeshEffectDescription& description) {
    vfx_world.addMeshEffect(description);
}

VFXWorld::ParticleBatch RenderWorld::getParticles() const {
    return vfx_world.getParticles();
}

VFXWorld::MeshParticleBatch RenderWorld::getMeshParticles() const {
    return vfx_world.getMeshParticles();
}

CustomRenderObject RenderWorld::addCustomRenderObject(
    const CustomRenderObjectCreateData& data) {
    auto buffer = BufferBuilder(data.buffer_size)
                      .isConstantBuffer()
                      .isChained()
                      .isCPUWritable(true, true)
                      .create(device)
                      .getResult();

    return CustomRenderObject(custom_ro_registry.create(
        data.model, data.pipeline, data.mesh, buffer));
}

std::vector<Handle<CustomRenderObjectData>>
RenderWorld::getCustomRenderObjects() {
    return custom_ro_registry.getAllocatedObjects();
}

void RenderWorld::update() { vfx_world.update(render_data.delta_time); }

}  // namespace Graphics