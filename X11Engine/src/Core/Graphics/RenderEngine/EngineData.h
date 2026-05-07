#pragma once

#include "DescriptorSet.h"
#include "Device.h"
#include "ShaderRegistry.h"

namespace Graphics {

struct EngineData {
    Device& device;

    DescriptorSet& descriptor_set;
    ShaderRegistry& shader_registry;
};

}  // namespace Graphics