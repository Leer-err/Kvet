#include "MeshRegistry.h"

#include <optional>

namespace Graphics {

MeshRegistry::MeshRegistry() : next_handle(0) {}

MeshHandle MeshRegistry::addMesh(const Mesh& mesh) {
    meshes.push_back(mesh);
    auto handle = next_handle;
    next_handle++;

    return handle;
}

std::optional<Mesh> MeshRegistry::getMesh(MeshHandle handle) {
    if (handle >= meshes.size()) {
        return {};
    }

    return meshes.at(handle);
}

}  // namespace Graphics