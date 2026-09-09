#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <cstdint>

#include "Buffer.h"
#include "Descriptors.h"
#include "IndexAllocator.h"
#include "Result.h"
#include "TextureState.h"

namespace Graphics {

class Texture {
   public:
    struct AllocatedTexture {
        VkImage texture;
        VmaAllocation allocation;
        VkImageView view;

        TextureDescriptor descriptor;
    };

    static Result<Texture, TextureError> create(
        VkDevice device, VmaAllocator allocator,
        const VkImageCreateInfo& image_info,
        const VmaAllocationCreateInfo& alloc_info,
        IndexAllocator<TextureDescriptor>& index_allocator);

    Texture(VmaAllocator allocator, const AllocatedTexture& allocated_image,
            const VkImageCreateInfo& image_info,
            const VmaAllocationCreateInfo& alloc_info,
            IndexAllocator<TextureDescriptor>& index_allocator);
    ~Texture();

    Texture& operator=(Texture&& other) noexcept;
    Texture(Texture&& other) noexcept;

    VkImageMemoryBarrier2 createBarrier(VkImageLayout new_layout,
                                        VkPipelineStageFlags2 src_stages,
                                        VkAccessFlags2 src_access,
                                        VkPipelineStageFlags2 dst_stages,
                                        VkAccessFlags2 dst_access);

    uint32_t getWidth() const;
    uint32_t getHeight() const;
    VkImage getImage() const;
    VkFormat getFormat() const;
    VkImageLayout getLayout() const;
    VkImageView getView() const;
    TextureDescriptor getDescriptor() const;

   private:
    VmaAllocator allocator;

    VkImage texture;
    VmaAllocation allocation;
    VkImageLayout layout;

    VkImageView view;
    TextureDescriptor descriptor;
    IndexAllocator<TextureDescriptor>& index_allocator;

    VkFormat format;
    uint32_t width;
    uint32_t height;
};

}  // namespace Graphics