#pragma once

#include "GraphicsResourceManager.h"
#include "MeshRegistry.h"
#include "Registries.h"
#include "ShaderRegistry.h"
#include "StagingBuffer.h"

namespace Graphics {

struct EngineData {
    ShaderRegistry& shader_registry;
    MeshRegistry& mesh_registry;

    StagingBuffer& staging_buffer;

    GraphicsResourceManager& resource_manager;
};

}  // namespace Graphics