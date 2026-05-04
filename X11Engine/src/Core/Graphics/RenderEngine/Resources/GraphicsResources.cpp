#include "GraphicsResources.h"

// #include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <VkBootstrap.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "DeviceProperties.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "Window.h"

namespace Graphics {

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

Resources::Resources() : logger(LoggerFactory::getLogger("Graphics")) {
    checkExtensions();
    createInstance();
    createDevice();
    createQueues();
    createAllocator();
}

void Resources::createInstance() {
    vkb::InstanceBuilder builder;
    auto inst_ret = builder.set_app_name("Example Vulkan Application")
                        .require_api_version(1, 3, 0)
                        .request_validation_layers()
                        .use_default_debug_messenger()
                        .build();

    if (!inst_ret) {
        logger.error("Vulkan instance creation failed with {}",
                     inst_ret.error().message());
        return;
    }
    instance = inst_ret.value();
}

void Resources::createDevice() {
    VkSurfaceKHR surface;
    auto handle = Window::get().getHandle();
    SDL_Vulkan_CreateSurface(handle, instance.instance, nullptr, &surface);

    vkb::PhysicalDeviceSelector selector{instance};
    auto phys_ret =
        selector.set_surface(surface)
            .set_minimum_version(1, 3)
            .require_dedicated_transfer_queue()
            .add_required_extension(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME)
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

    VkPhysicalDeviceVulkan12Features features12 = {};
    features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    features12.bufferDeviceAddress = VK_TRUE;
    features12.runtimeDescriptorArray = VK_TRUE;
    device_builder.add_pNext(&features12);

    VkPhysicalDeviceSynchronization2FeaturesKHR synchronization_2{
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR};
    synchronization_2.synchronization2 = VK_TRUE;
    device_builder.add_pNext(&synchronization_2);

    VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_feature{};
    dynamic_rendering_feature.sType =
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
    dynamic_rendering_feature.dynamicRendering = VK_TRUE;
    device_builder.add_pNext(&dynamic_rendering_feature);

    VkPhysicalDeviceDescriptorBufferFeaturesEXT descriptor_buffer{
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT};
    descriptor_buffer.descriptorBuffer = VK_TRUE;
    device_builder.add_pNext(&descriptor_buffer);

    auto dev_ret = device_builder.build();
    if (!dev_ret) {
        logger.error("Failed to find suitable device");
        return;
    }
    device = dev_ret.value();

    readProperties(*phys_ret);
}

void Resources::readProperties(const vkb::PhysicalDevice& device) {
    properties = DeviceProperties::readProperties(device.physical_device);

    logger.info("Using device {}", properties.device_name);
    logger.info("Device dedicated memory {} MB",
                properties.memory_size / (1 << 20));
}

void Resources::checkExtensions() {
    auto system_info_ret = vkb::SystemInfo::get_system_info();
    if (!system_info_ret) {
    }
    auto system_info = system_info_ret.value();
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
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

    VkResult result = vmaCreateAllocator(&allocatorCreateInfo, &allocator);
    if (result != VK_SUCCESS)
        logger.error("Vulkan allocator was not created with {}",
                     string_VkResult(result));
}

uint32_t Resources::getGraphicsQueueIndex() const {
    return device.get_queue_index(vkb::QueueType::graphics).value();
}

vkb::Device Resources::getVKBDevice() const { return device; }

VkDevice Resources::getDevice() const { return device.device; }

VkQueue Resources::getGraphicsQueue() const { return graphics_queue; }

VkQueue Resources::getPresentationQueue() const { return presentation_queue; }

VmaAllocator Resources::getAllocator() const { return allocator; }

DeviceProperties Resources::getProperties() const { return properties; }

}  // namespace Graphics