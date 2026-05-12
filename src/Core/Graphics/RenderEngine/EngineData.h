#pragma once

#include "DescriptorSet.h"
#include "Device.h"
#include "MeshRegistry.h"
#include "ShaderRegistry.h"
#include "StagingBuffer.h"

namespace Graphics {

struct EngineData {
    Device& device;

    DescriptorSet& descriptor_set;
    ShaderRegistry& shader_registry;
    MeshRegistry& mesh_registry;

    StagingBuffer& staging_buffer;
};

}  // namespace Graphics