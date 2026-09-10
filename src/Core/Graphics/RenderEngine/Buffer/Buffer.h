#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <span>

#include "EngineConstants.h"
#include "Result.h"

namespace Graphics {

enum class BufferError { OutOfMemory };

class Buffer {
   public:
    struct AllocatedBuffer {
        VkBuffer buffer;
        VmaAllocation allocation;

        uint8_t* mapped_address;
        VkDeviceAddress device_address;
    };

   private:
    using BufferChain = std::array<AllocatedBuffer, MAX_FRAMES_IN_FLIGHT>;

   public:
    static Result<Buffer, BufferError> create(
        VkDevice device, VmaAllocator allocator,
        const VkBufferCreateInfo& buffer_info,
        const VmaAllocationCreateInfo& alloc_info, bool is_chained);

    Buffer(VmaAllocator allocator, const BufferChain& allocated_buffer,
           size_t size, const VkBufferCreateInfo& buffer_info,
           const VmaAllocationCreateInfo& alloc_info);
    ~Buffer();

    Buffer& operator=(Buffer&& other) noexcept;
    Buffer(Buffer&& other) noexcept;

    template <typename T>
    void update(const T& data, size_t offset = 0) {
        update(std::bit_cast<uint8_t*>(&data), sizeof(T), offset);
    }
    void update(const uint8_t* data, size_t size, size_t offset = 0);

    VkBuffer getHandle() const;

    template <typename T>
    T* getHostAddress() const {
        return std::bit_cast<T*>(getHostAddress());
    }
    uint8_t* getHostAddress() const;
    VkDeviceAddress getDeviceAddress() const;

    VkBufferMemoryBarrier2 createBarrier(VkPipelineStageFlags2 src_stages,
                                         VkAccessFlags2 src_access,
                                         VkPipelineStageFlags2 dst_stages,
                                         VkAccessFlags2 dst_access);

    static void setFrameInFlightIndex(uint32_t index);

   private:
    static uint32_t frame_in_flight_index;

    static Result<AllocatedBuffer, BufferError> allocateSingleBuffer(
        VkDevice device, VmaAllocator allocator,
        const VkBufferCreateInfo& buffer_info,
        const VmaAllocationCreateInfo& alloc_info);

    AllocatedBuffer& getCurrentBuffer();
    const AllocatedBuffer& getCurrentBuffer() const;

    VmaAllocator allocator;

    BufferChain buffers;
    size_t size;
};

}  // namespace Graphics