#include "Buffer.h"

#include "GraphicsResources.h"

namespace Graphics {

void Buffer::destroy() {
    if (buffer != VK_NULL_HANDLE) {
        auto allocator = Resources::get().getAllocator();

        vmaDestroyBuffer(allocator, buffer, allocation);
    }
}

size_t Buffer::getSize() const { return size; }

void* Buffer::map() {
    auto allocator = Resources::get().getAllocator();

    void* mapped_data;
    vmaMapMemory(allocator, allocation, &mapped_data);

    return mapped_data;
}

void Buffer::unmap() {
    auto allocator = Resources::get().getAllocator();

    vmaUnmapMemory(allocator, allocation);
}

VkDeviceAddress Buffer::getDeviceAddress() const {
    auto device = Resources::get().getDevice();

    VkBufferDeviceAddressInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
    info.buffer = buffer;
    return vkGetBufferDeviceAddress(device, &info);
}

}  // namespace Graphics