#include "GraphicsResources.h"

// #include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>

#include "Logger.h"
#include "LoggerFactory.h"
#include "Window.h"

namespace Graphics {

Resources::Resources() : logger(LoggerFactory::getLogger("Graphics")) {
    createInstance();
    createDevice();
    getQueue();
}

void Resources::createInstance() {
    vkb::InstanceBuilder builder;
    auto inst_ret = builder.set_app_name("Example Vulkan Application")
                        .request_validation_layers()
                        .use_default_debug_messenger()
                        .build();

    if (!inst_ret) {
        logger.error("Vulkan instance creation failed");
        return;
    }
    instance = inst_ret.value();
}

void Resources::createDevice() {
    VkSurfaceKHR surface;
    auto handle = Window::get().getHandle();
    SDL_Vulkan_CreateSurface(handle, instance.instance, nullptr, &surface);

    vkb::PhysicalDeviceSelector selector{instance};
    auto phys_ret = selector.set_surface(surface)
                        .set_minimum_version(1, 1)
                        .require_dedicated_transfer_queue()
                        .select();

    if (!phys_ret) {
        logger.error("Failed to find suitable device");
        if (phys_ret.error() == vkb::PhysicalDeviceError::no_suitable_device) {
            const auto& detailed_reasons = phys_ret.detailed_failure_reasons();
            if (!detailed_reasons.empty()) {
                logger.debug("GPU Selection failure reasons:");
                for (const std::string& reason : detailed_reasons)
                    logger.debug("{}", reason);
            }
        }
        return;
    }

    vkb::DeviceBuilder device_builder{phys_ret.value()};
    auto dev_ret = device_builder.build();
    if (!dev_ret) {
        logger.error("Failed to find suitable device");
        return;
    }
    device = dev_ret.value();

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(phys_ret->physical_device,
                                  &device_properties);
    logger.info("Using device {}", device_properties.deviceName);
}

void Resources::createQueue() {
    auto graphics_queue_ret = device.get_queue(vkb::QueueType::graphics);
    if (!graphics_queue_ret) logger.error("Failure requesting graphics queue");
    queue = graphics_queue_ret.value();
}

vkb::Device Resources::getDevice() const { return device; }

VkQueue Resources::getQueue() const { return queue; }

}  // namespace Graphics