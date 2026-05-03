#pragma once

#include <VkBootstrap.h>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include "CommandPool.h"
#include "DeviceProperties.h"
#include "Fence.h"
#include "Logger.h"
#include "Semaphore.h"

namespace Graphics {

class Resources {
   public:
    static Resources& get() {
        static Resources instance;
        return instance;
    }

    VkDevice getDevice() const;
    VkQueue getGraphicsQueue() const;
    uint32_t getGraphicsQueueIndex() const;
    VkQueue getPresentationQueue() const;
    VmaAllocator getAllocator() const;
    DeviceProperties getProperties() const;

    vkb::Device getVKBDevice() const;

   private:
    Resources();
    Resources(const Resources&) = delete;
    Resources& operator=(const Resources&) = delete;
    Resources(Resources&&) = delete;
    Resources& operator=(Resources&&) = delete;

    void createInstance();
    void createDevice();
    void createQueues();

    void checkExtensions();

    void readProperties(const vkb::PhysicalDevice& device);

    void createAllocator();

    vkb::Instance instance;
    vkb::Device device;
    VkQueue graphics_queue;
    VkQueue presentation_queue;
    VmaAllocator allocator;

    DeviceProperties properties;

    Logger logger;
};

}  // namespace Graphics