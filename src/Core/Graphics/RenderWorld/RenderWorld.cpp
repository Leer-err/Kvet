#include "RenderWorld.h"

#include <span>

#include "Texture.h"
#include "VFXWorld.h"

namespace Graphics {

RenderWorld::RenderWorld() : next_handle(0), vfx_world(10000) {}

RenderData& RenderWorld::renderData() { return render_data; }

const RenderData& RenderWorld::renderData() const { return render_data; }

OpaqueObjectHandle RenderWorld::addOpaqueObject(const RenderObjectData& data) {
    opaque_objects.emplace_back(data);

    return next_handle++;
}

RenderObjectData& RenderWorld::getOpaqueObject(
    const OpaqueObjectHandle& handle) {
    return opaque_objects.at(handle);
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

void RenderWorld::update(float delta_time) { vfx_world.update(delta_time); }

}  // namespace Graphics