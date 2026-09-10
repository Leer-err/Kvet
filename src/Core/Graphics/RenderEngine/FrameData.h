#pragma once

#include "CommandBuffer.h"
#include "Device.h"

namespace Graphics {

struct FrameData {
    TracyVkCtx trace_ctx;

    CommandBuffer cmd;
};

}  // namespace Graphics