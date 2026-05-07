#include "Device.h"

#include "APIData.h"
#include "Buffer.h"
#include "Image.h"

namespace Graphics {

Result<Image, ImageError> Device::createImage(
    const VkImageCreateInfo& image_info,
    const VmaAllocationCreateInfo& alloc_info) const {
    VkImage image;
    VmaAllocation allocation;
    vmaCreateImage(allocator, &image_info, &alloc_info, &image, &allocation,
                   nullptr);

    auto result = Image{};
    result.image = image;
    result.allocation = allocation;
    result.layout = VK_IMAGE_LAYOUT_UNDEFINED;
    result.format = image_info.format;
    result.width = image_info.extent.width;
    result.height = image_info.extent.height;

    return result;
}

Result<Buffer, BufferError> Device::createBuffer(
    const VkBufferCreateInfo& buffer_info,
    const VmaAllocationCreateInfo& alloc_info) const {
    Buffer buffer = {};

    vmaCreateBuffer(allocator, &buffer_info, &alloc_info, &buffer.buffer,
                    &buffer.allocation, nullptr);

    VkBufferDeviceAddressInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
    info.buffer = buffer.buffer;
    buffer.device_address = vkGetBufferDeviceAddress(device, &info);

    buffer.size = buffer_info.size;
    return buffer;
}

void* Device::map(const Buffer& buffer) const {
    void* data;
    vmaMapMemory(allocator, buffer.allocation, &data);
    return data;
}

void Device::unmap(const Buffer& buffer) const {
    vmaUnmapMemory(allocator, buffer.allocation);
}

VkCommandPool Device::createCommandPool(uint32_t queue_index) const {
    VkCommandPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = queue_index;

    VkCommandPool pool = {};
    vkCreateCommandPool(device, &info, nullptr, &pool);
    return pool;
}

VkCommandBuffer Device::createCommandBuffer(VkCommandPool pool) const {}

}  // namespace Graphics