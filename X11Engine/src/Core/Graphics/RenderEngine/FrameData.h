#pragma once

#include "CommandBuffer.h"
#include "DescriptorSet.h"
#include "RenderEnviroment.h"

namespace Graphics {

struct FrameData {
    CommandBuffer cmd;
    RenderEnviroment env;

    DescriptorSet set;
};

}  // namespace Graphics