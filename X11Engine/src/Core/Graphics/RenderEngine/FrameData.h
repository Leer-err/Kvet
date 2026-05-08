#pragma once

#include "CommandBuffer.h"
#include "DescriptorSet.h"
#include "RenderEnviroment.h"

namespace Graphics {

struct FrameData {
    CommandBuffer cmd;
    RenderEnviroment env;

    DescriptorSet& descriptor_set;
};

}  // namespace Graphics