#pragma once

#include "CommandBuffer.h"
#include "DescriptorSet.h"
#include "Device.h"
#include "RenderEnviroment.h"

namespace Graphics {

struct FrameData {
    uint32_t frame_in_flight_index;

    CommandBuffer cmd;
    RenderEnviroment env;

    DescriptorSet& descriptor_set;

    TracyVkCtx trace_ctx;
};

}  // namespace Graphics