#include "BufferBuilder.h"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

#include <memory>

#include "Buffer.h"
#include "GraphicsResources.h"
#include "InternalBuffer.h"
#include "Resources.h"

namespace Graphics {

BufferBuilder::BufferBuilder(size_t size)
    : size(size),
      stride(0),
      offset(0),
      shader_resource(false),
      vertex_buffer(false),
      index_buffer(false),
      constant_buffer(false),
      cpu_writable(false),
      gpu_writable(false),
      data(nullptr) {}

BufferBuilder& BufferBuilder::withData(const void* data) {
    this->data = data;
    return *this;
}

BufferBuilder& BufferBuilder::isShaderResource() {
    shader_resource = true;
    return *this;
}

BufferBuilder& BufferBuilder::isVertexBuffer(size_t stride, size_t offset) {
    this->stride = stride;
    this->offset = offset;

    vertex_buffer = true;
    return *this;
}

BufferBuilder& BufferBuilder::isIndexBuffer() {
    index_buffer = true;
    return *this;
}

BufferBuilder& BufferBuilder::isConstantBuffer() {
    constant_buffer = true;
    return *this;
}

BufferBuilder& BufferBuilder::isCPUWritable() {
    cpu_writable = true;
    return *this;
}

BufferBuilder& BufferBuilder::isGPUWritable() {
    gpu_writable = true;
    return *this;
}

Result<Buffer, BufferError> BufferBuilder::create() {
    // Validation
    if (cpu_writable && gpu_writable) return BufferError::WriteFromGPUAndCPU;
    if (data == nullptr && !gpu_writable && !cpu_writable)
        return BufferError::NoDataForImmutableResource;

    VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    if (shader_resource)
        buffer_info.usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    if (constant_buffer)
        buffer_info.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    if (vertex_buffer) buffer_info.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    if (index_buffer) buffer_info.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    if (gpu_writable) buffer_info.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
    if (cpu_writable)
        alloc_info.flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;

    VkBuffer buffer;
    VmaAllocation allocation;
    vmaCreateBuffer(Resources::get().getAllocator(), &buffer_info, &alloc_info,
                    &buffer, &allocation, nullptr);

    auto internal = std::make_unique<Internal::Buffer>();
    internal->buffer = buffer;
    internal->size = size;

    return Buffer(std::move(internal));
}

}  // namespace Graphics