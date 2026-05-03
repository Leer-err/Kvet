#include "BufferBuilder.h"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <utility>

#include "Buffer.h"
#include "GraphicsResources.h"
#include "Result.h"

namespace Graphics {

BufferBuilder::BufferBuilder(size_t size)
    : size(size),
      stride(0),
      offset(0),
      shader_resource(false),
      vertex_buffer(false),
      index_buffer(false),
      constant_buffer(false),
      cpu_write_random(false),
      cpu_write_sequential(false),
      copy_source(false),
      copy_target(false) {}

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

BufferBuilder& BufferBuilder::isDescriptorBuffer() {
    descriptor_buffer = true;
    return *this;
}

BufferBuilder& BufferBuilder::isCPUWritable(bool is_random) {
    if (is_random)
        cpu_write_random = true;
    else
        cpu_write_sequential = true;

    return *this;
}

BufferBuilder& BufferBuilder::isCopySource() {
    copy_source = true;
    return *this;
}

BufferBuilder& BufferBuilder::isCopyDestination() {
    copy_target = true;
    return *this;
}

Result<Buffer, BufferError> BufferBuilder::create() {
    VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.pNext = nullptr;
    buffer_info.size = size;
    if (shader_resource)
        buffer_info.usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    if (constant_buffer)
        buffer_info.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
                             VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
    if (vertex_buffer) buffer_info.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    if (index_buffer) buffer_info.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    if (descriptor_buffer)
        buffer_info.usage |=
            VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT |
            VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

    if (copy_source) buffer_info.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    if (copy_target) buffer_info.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
    if (cpu_write_sequential)
        alloc_info.flags |=
            VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
    if (cpu_write_random)
        alloc_info.flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;

    VkBuffer buffer;
    VmaAllocation allocation;
    vmaCreateBuffer(Resources::get().getAllocator(), &buffer_info, &alloc_info,
                    &buffer, &allocation, nullptr);

    auto result = Buffer{};
    result.buffer = buffer;
    result.allocation = allocation;
    result.size = size;

    return result;
}

}  // namespace Graphics