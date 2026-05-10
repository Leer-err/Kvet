#pragma once

#include <cstddef>
#include <vector>

#include "Buffer.h"
#include "CommandBuffer.h"
#include "Device.h"
#include "Image.h"

namespace Graphics {

class StagingBuffer {
    struct BufferData {
        Buffer buffer;
        size_t host_offset;

        size_t data_size;
    };

    struct ImageData {
        Image image;
        size_t host_offset;

        size_t data_size;
    };

   public:
    StagingBuffer(Device& device);

    void stageImage(const Image& destination, const void* data,
                    size_t data_size);
    void stageBuffer(const Buffer& destination, const void* data,
                     size_t data_size);

    void flush(const CommandBuffer& cmd);

   private:
    Device& device;

    std::vector<char> host_data_buffer;
    size_t host_data_used;

    Buffer buffer;

    std::vector<BufferData> buffers;
    std::vector<ImageData> images;
};

}  // namespace Graphics