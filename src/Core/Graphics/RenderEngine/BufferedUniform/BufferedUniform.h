#pragma once

#include <vulkan/vulkan_core.h>

#include <array>
#include <cstddef>

#include "Buffer.h"
#include "EngineConstants.h"
#include "EngineData.h"
#include "FrameData.h"

namespace Graphics {

class BufferedUniformBase {
   public:
    BufferedUniformBase(EngineData& engine_data, size_t size);

    void update(const FrameData& frame, const void* data);

    const Buffer& getBuffer(const FrameData& frame);
    VkDeviceAddress getAddress(const FrameData& frame);

   private:
    EngineData& engine_data;

    std::array<Buffer, MAX_FRAMES_IN_FLIGHT> buffers;
    size_t size;
};

template <typename T>
class BufferedUniform {
   public:
    BufferedUniform(EngineData& engine_data) : base(engine_data, sizeof(T)) {}

    void update(const FrameData& frame, const T& data) {
        base.update(frame, &data);
    }

    const Buffer& getBuffer(const FrameData& frame) {
        return base.getBuffer(frame);
    }

    VkDeviceAddress getAddress(const FrameData& frame) {
        return base.getAddress(frame);
    }

   private:
    BufferedUniformBase base;
};

}  // namespace Graphics