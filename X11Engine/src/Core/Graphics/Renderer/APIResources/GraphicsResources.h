#pragma once

#include <vulkan/vulkan_core.h>

#include "Logger.h"
#include "VkBootstrap.h"

namespace Graphics {

class Resources {
   public:
    static Resources& get() {
        static Resources instance;
        return instance;
    }

    vkb::Device getDevice() const;
    VkQueue getQueue() const;

   private:
    Resources();
    Resources(const Resources&) = delete;
    Resources& operator=(const Resources&) = delete;
    Resources(Resources&&) = delete;
    Resources& operator=(Resources&&) = delete;

    void createInstance();
    void createDevice();
    void createQueue();

    vkb::Instance instance;
    vkb::Device device;
    VkQueue queue;

    Logger logger;
};

}  // namespace Graphics