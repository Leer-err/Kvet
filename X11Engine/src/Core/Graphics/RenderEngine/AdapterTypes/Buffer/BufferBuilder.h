#pragma once

#include <cstddef>

#include "Buffer.h"
#include "EngineData.h"
#include "Result.h"

namespace Graphics {

class BufferBuilder {
   public:
    BufferBuilder(const EngineData& engine_data, size_t size);

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
    EngineData engine_data;

    VkBufferCreateInfo buffer_info;
    VmaAllocationCreateInfo alloc_info;
};

}  // namespace Graphics