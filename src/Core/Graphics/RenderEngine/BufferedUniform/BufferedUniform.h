#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "Buffer.h"
#include "Device.h"
#include "FrameData.h"

namespace Graphics {

class BufferedUniformBase {
   public:
    BufferedUniformBase(const Device& device, size_t size);
    ~BufferedUniformBase();

    void update(const FrameData& frame, const void* data);

    Buffer getBuffer(const FrameData& frame);

   private:
    std::array<Buffer, MAX_FRAMES_IN_FLIGHT> buffers;
};

template <typename T>
class BufferedUniform {
   public:
    BufferedUniform(const Device& device) : base(device, sizeof(T)) {}

    void update(const FrameData& frame, const T& data) {
        base.update(frame, &data);
    }

    Buffer getBuffer(const FrameData& frame) { return base.getBuffer(frame); }

   private:
    BufferedUniformBase base;
};

}  // namespace Graphics