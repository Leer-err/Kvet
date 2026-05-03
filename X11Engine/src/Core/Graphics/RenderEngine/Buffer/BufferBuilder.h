#pragma once

#include <cstddef>

#include "Buffer.h"
#include "Result.h"

namespace Graphics {

enum class BufferError { NoDataForImmutableResource, WriteFromGPUAndCPU };

class BufferBuilder {
   public:
    BufferBuilder(size_t size);

    BufferBuilder& isShaderResource();
    BufferBuilder& isVertexBuffer(size_t stride, size_t offset = 0);
    BufferBuilder& isIndexBuffer();
    BufferBuilder& isConstantBuffer();
    BufferBuilder& isDescriptorBuffer();

    BufferBuilder& isCPUWritable(bool is_random = false);

    BufferBuilder& isCopySource();
    BufferBuilder& isCopyDestination();

    Result<Buffer, BufferError> create();

   private:
    size_t size;
    size_t stride;
    size_t offset;

    bool shader_resource;
    bool vertex_buffer;
    bool index_buffer;
    bool constant_buffer;
    bool descriptor_buffer;

    bool cpu_write_random;
    bool cpu_write_sequential;

    bool copy_source;
    bool copy_target;
};

}  // namespace Graphics