#include "Buffer.h"

#include <memory>

#include "GraphicsResources.h"
#include "InternalBuffer.h"

namespace Graphics {

Buffer::~Buffer() = default;

Buffer::Buffer(Buffer&&) = default;

Buffer& Buffer::operator=(Buffer&&) = default;

Buffer::Buffer() { buffer = std::make_unique<Internal::Buffer>(); }

Buffer::Buffer(Internal::Buffer&& buffer)
    : buffer(std::make_unique<Internal::Buffer>(std::move(buffer))) {}

Internal::Buffer* Buffer::getInternal() const { return buffer.get(); }

size_t Buffer::getSize() const { return buffer->size; }

void* Buffer::map() {
    auto allocator = Resources::get().getAllocator();

    void* mapped_data;
    vmaMapMemory(allocator, buffer->allocation, &mapped_data);

    return mapped_data;
}

void Buffer::unmap() {
    auto allocator = Resources::get().getAllocator();

    vmaUnmapMemory(allocator, buffer->allocation);
}

}  // namespace Graphics