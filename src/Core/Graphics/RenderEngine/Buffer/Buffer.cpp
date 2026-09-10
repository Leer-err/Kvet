#include "Buffer.h"

#include <vulkan/vulkan.h>

#include <array>
#include <cstring>

#include "EngineConstants.h"

namespace Graphics {

uint32_t Buffer::frame_in_flight_index = 0;

Result<Buffer, BufferError> Buffer::create(
    VkDevice device, VmaAllocator allocator,
    const VkBufferCreateInfo& buffer_info,
    const VmaAllocationCreateInfo& alloc_info, bool is_chained) {
    auto buffers = BufferChain{};

    if (is_chained) {
        for (auto& buffer : buffers) {
            auto buffer_result = allocateSingleBuffer(device, allocator,
                                                      buffer_info, alloc_info);

            if (buffer_result.isError()) return buffer_result.getError();

            buffer = buffer_result.getResult();
        }
    } else {
        auto buffer_result =
            allocateSingleBuffer(device, allocator, buffer_info, alloc_info);

        if (buffer_result.isError()) return buffer_result.getError();

        for (auto& buffer : buffers) buffer = buffer_result.getResult();
    }

    return Buffer(allocator, buffers, buffer_info.size, buffer_info,
                  alloc_info);
}

Buffer::Buffer(VmaAllocator allocator, const BufferChain& allocated_buffers,
               size_t size, const VkBufferCreateInfo& buffer_info,
               const VmaAllocationCreateInfo& alloc_info)
    : allocator(allocator), buffers(allocated_buffers), size(size) {}

Buffer::~Buffer() {
    VkBuffer prev_buffer = VK_NULL_HANDLE;

    for (const auto& buffer : buffers) {
        if (buffer.buffer == VK_NULL_HANDLE || buffer.buffer == prev_buffer)
            break;

        vmaDestroyBuffer(allocator, buffer.buffer, buffer.allocation);
        prev_buffer = buffer.buffer;
    }
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
    buffers = other.buffers;
    size = other.size;
    allocator = other.allocator;

    for (auto& buffer : other.buffers) buffer.buffer = VK_NULL_HANDLE;
    return *this;
}

Buffer::Buffer(Buffer&& other) noexcept
    : allocator(other.allocator), buffers(other.buffers), size(other.size) {
    for (auto& buffer : other.buffers) buffer.buffer = VK_NULL_HANDLE;
}

void Buffer::update(const uint8_t* data, size_t size, size_t offset) {
    auto mapped_address = getHostAddress();
    memcpy(mapped_address + offset, data, size);
}

VkBuffer Buffer::getHandle() const { return getCurrentBuffer().buffer; }

uint8_t* Buffer::getHostAddress() const {
    return getCurrentBuffer().mapped_address;
}

VkDeviceAddress Buffer::getDeviceAddress() const {
    return getCurrentBuffer().device_address;
}

Buffer::AllocatedBuffer& Buffer::getCurrentBuffer() { return buffers[0]; }

const Buffer::AllocatedBuffer& Buffer::getCurrentBuffer() const {
    return buffers[frame_in_flight_index];
}

VkBufferMemoryBarrier2 Buffer::createBarrier(VkPipelineStageFlags2 src_stages,
                                             VkAccessFlags2 src_access,
                                             VkPipelineStageFlags2 dst_stages,
                                             VkAccessFlags2 dst_access) {
    VkBufferMemoryBarrier2 barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;
    barrier.buffer = getCurrentBuffer().buffer;
    barrier.srcStageMask = src_stages;
    barrier.srcAccessMask = src_access;
    barrier.dstStageMask = dst_stages;
    barrier.dstAccessMask = dst_access;
    barrier.size = VK_WHOLE_SIZE;

    return barrier;
}

Result<Buffer::AllocatedBuffer, BufferError> Buffer::allocateSingleBuffer(
    VkDevice device, VmaAllocator allocator,
    const VkBufferCreateInfo& buffer_info,
    const VmaAllocationCreateInfo& alloc_info) {
    auto buffer = AllocatedBuffer{};

    VmaAllocationInfo info = {};
    VkResult result =
        vmaCreateBuffer(allocator, &buffer_info, &alloc_info, &buffer.buffer,
                        &buffer.allocation, &info);

    if (result != VK_SUCCESS) return BufferError{};

    if ((buffer_info.usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT) != 0) {
        VkBufferDeviceAddressInfo address_info = {};
        address_info.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        address_info.buffer = buffer.buffer;
        buffer.device_address = vkGetBufferDeviceAddress(device, &address_info);
    }

    if ((alloc_info.flags & VMA_ALLOCATION_CREATE_MAPPED_BIT) != 0)
        buffer.mapped_address = std::bit_cast<uint8_t*>(info.pMappedData);

    return buffer;
}

void Buffer::setFrameInFlightIndex(uint32_t index) {
    frame_in_flight_index = index;
}

}  // namespace Graphics