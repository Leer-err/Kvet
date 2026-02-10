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
    VkQueue getQueue() const;
    VmaAllocator getAllocator() const;

   private:
    Resources();
    Resources(const Resources&) = delete;
    Resources& operator=(const Resources&) = delete;
    Resources(Resources&&) = delete;
    Resources& operator=(Resources&&) = delete;

    void createInstance();
    void createDevice();
    void createQueue();

    void createAllocator();

    vkb::Instance instance;
    vkb::Device device;
    VkQueue queue;
    VmaAllocator allocator;

    Logger logger;
};

}  // namespace Graphics