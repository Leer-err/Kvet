#pragma once

#include <VkBootstrap.h>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

#include "Logger.h"

namespace Graphics {

class Resources {
   public:
    static Resources& get() {
        static Resources instance;
        return instance;
    }

    VkDevice getDevice() const;
    VkQueue getGraphicsQueue() const;
    VkQueue getPresentationQueue() const;
    VmaAllocator getAllocator() const;

   private:
    Resources();
    Resources(const Resources&) = delete;
    Resources& operator=(const Resources&) = delete;
    Resources(Resources&&) = delete;
    Resources& operator=(Resources&&) = delete;

    void createInstance();
    void createDevice();
    void createQueues();

    void createAllocator();

    vkb::Instance instance;
    vkb::Device device;
    VkQueue graphics_queue;
    VkQueue presentation_queue;
    VmaAllocator allocator;

    Logger logger;
};

}  // namespace Graphics