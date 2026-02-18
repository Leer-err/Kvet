#include "GraphicsResources.h"

// #include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <VkBootstrap.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "Logger.h"
#include "LoggerFactory.h"
#include "Window.h"

namespace Graphics {

Resources::Resources() : logger(LoggerFactory::getLogger("Graphics")) {
    createInstance();
    createDevice();
    createQueues();
    createAllocator();
}

void Resources::createInstance() {
    vkb::InstanceBuilder builder;
    auto inst_ret = builder.set_app_name("Example Vulkan Application")
                        .require_api_version(1, 1, 0)
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

void Resources::createQueues() {
    auto graphics_queue_ret = device.get_queue(vkb::QueueType::graphics);
    if (!graphics_queue_ret) logger.error("Failure requesting graphics queue");
    graphics_queue = graphics_queue_ret.value();

    auto presentation_queue_ret = device.get_queue(vkb::QueueType::present);
    if (!presentation_queue_ret)
        logger.error("Failure requesting presentation queue");
    presentation_queue = presentation_queue_ret.value();
}

void Resources::createAllocator() {
    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_1;
    allocatorCreateInfo.physicalDevice = device.physical_device;
    allocatorCreateInfo.device = device.device;
    allocatorCreateInfo.instance = instance.instance;

    VkResult result = vmaCreateAllocator(&allocatorCreateInfo, &allocator);
    if (result != VK_SUCCESS)
        logger.error("Vulkan allocator was not created with {}",
                     string_VkResult(result));
}

VkDevice Resources::getDevice() const { return device.device; }

VkQueue Resources::getGraphicsQueue() const { return graphics_queue; }

VkQueue Resources::getPresentationQueue() const { return presentation_queue; }

VmaAllocator Resources::getAllocator() const { return allocator; }

}  // namespace Graphics