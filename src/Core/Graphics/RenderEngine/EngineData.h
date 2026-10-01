#pragma once

#include "GraphicsResourceManager.h"
#include "StagingBuffer.h"

namespace Graphics {

struct EngineData {
    StagingBuffer& staging_buffer;

    ResourceManager& resource_manager;
};

}  // namespace Graphics