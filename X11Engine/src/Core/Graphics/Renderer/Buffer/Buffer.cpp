#include "Buffer.h"

#include "InternalBuffer.h"

namespace Graphics {

Buffer::Buffer() { buffer = std::make_unique<Internal::Buffer>(); }

Buffer::Buffer(std::unique_ptr<Internal::Buffer>&& buffer)
    : buffer(std::move(buffer)) {}

Internal::Buffer* Buffer::getInternal() const { return buffer.get(); }

size_t Buffer::getSize() const { return buffer->size; }

}  // namespace Graphics