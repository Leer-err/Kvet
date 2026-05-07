#include "BufferBuilder.h"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include "Buffer.h"
#include "Device.h"
#include "Result.h"

namespace Graphics {

BufferBuilder::BufferBuilder(const Device& device, size_t size)
    : device(device), buffer_info(), alloc_info() {
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
}

BufferBuilder& BufferBuilder::isShaderResource() {
    buffer_info.usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    return *this;
}

BufferBuilder& BufferBuilder::isVertexBuffer(size_t stride, size_t offset) {
    buffer_info.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    return *this;
}

BufferBuilder& BufferBuilder::isIndexBuffer() {
    buffer_info.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    return *this;
}

BufferBuilder& BufferBuilder::isConstantBuffer() {
    buffer_info.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
                         VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
    return *this;
}

BufferBuilder& BufferBuilder::isDescriptorBuffer() {
    buffer_info.usage |= VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT |
                         VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
    return *this;
}

BufferBuilder& BufferBuilder::isCPUWritable(bool is_random) {
    if (is_random)
        alloc_info.flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
    else
        alloc_info.flags |=
            VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

    return *this;
}

BufferBuilder& BufferBuilder::isCopySource() {
    buffer_info.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    return *this;
}

BufferBuilder& BufferBuilder::isCopyDestination() {
    buffer_info.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    return *this;
}

Result<Buffer, BufferError> BufferBuilder::create() {
    return device.createBuffer(buffer_info, alloc_info);
}

}  // namespace Graphics