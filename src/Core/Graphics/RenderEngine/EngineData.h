#pragma once

#include "GraphicsResourceManager.h"
#include "Registries.h"
#include "ShaderRegistry.h"
#include "StagingBuffer.h"

namespace Graphics {

struct EngineData {
    ShaderRegistry& shader_registry;

    StagingBuffer& staging_buffer;

    ResourceManager& resource_manager;
};

}  // namespace Graphics