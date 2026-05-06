#include "Graphics.h"

#include "AppConfig.h"
#include "EngineData.h"
#include "ExtensionFunctions.h"
#include "GraphicsResources.h"
#include "LoggerFactory.h"
#include "Result.h"
#include "SDL3/SDL_vulkan.h"
#include "VkBootstrap.h"
#include "Window.h"

namespace Graphics {

enum class Error {
    InstanceCreationFailed,
    NoSuitableDevice,
    DeviceCreationFailed,
    QueueCreationFailed,
    AllocatorCreationFailed
};

static Result<vkb::Instance, Error> createInstance(const Config::App& config) {
    vkb::InstanceBuilder builder;
    auto inst_ret = builder.set_app_name(config.getName().c_str())
                        .require_api_version(1, 3, 0)
                        .request_validation_layers()
                        .use_default_debug_messenger()
                        .build();

    if (!inst_ret) return Error::InstanceCreationFailed;

    return inst_ret.value();
};

static Result<vkb::Device, Error> createDevice(const vkb::Instance& instance) {
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
        if (phys_ret.error() == vkb::PhysicalDeviceError::no_suitable_device)
            return Error::NoSuitableDevice;
        return Error::DeviceCreationFailed;
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
    if (!dev_ret) return Error::DeviceCreationFailed;

    return dev_ret.value();
};

static Result<Queue, Error> createQueue(const vkb::Device& device,
                                        const vkb::QueueType& queue_type) {
    auto graphics_queue_ret = device.get_queue(queue_type);
    if (!graphics_queue_ret) return Error::QueueCreationFailed;

    auto index = device.get_queue_index(queue_type).value();

    return Queue{graphics_queue_ret.value(), index};
}

static Result<VmaAllocator, Error> createAllocator(
    const vkb::Instance& instance, const vkb::Device& device) {
    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
    allocatorCreateInfo.physicalDevice = device.physical_device;
    allocatorCreateInfo.device = device.device;
    allocatorCreateInfo.instance = instance.instance;
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

    VmaAllocator allocator = {};
    VkResult result = vmaCreateAllocator(&allocatorCreateInfo, &allocator);
    if (result != VK_SUCCESS) return Error::AllocatorCreationFailed;

    return allocator;
}

static void printError(const Error error) {
    auto logger = LoggerFactory::getLogger("Graphics");
    switch (error) {
        case Error::InstanceCreationFailed:
            logger.error("Vulkan instance creation failed");
            break;
        case Error::DeviceCreationFailed:
            logger.error("Vulkan device creation failed");
            break;
        case Error::QueueCreationFailed:
            logger.error("Failed to acquire queue from device");
            break;
        case Error::AllocatorCreationFailed:
            logger.error("Failed to create allocator");
            break;
        case Error::NoSuitableDevice:
            logger.error("Failed to find suitable device");
            break;
    }
}

bool init() {
    auto instance_result = createInstance(Config::App::get());
    if (instance_result.isError()) {
        printError(instance_result.getError());
        return false;
    }
    auto instance = instance_result.getResult();

    auto device_result = createDevice(instance);
    if (device_result.isError()) {
        printError(device_result.getError());
        return false;
    }
    auto device = device_result.getResult();

    auto graphics_queue_result = createQueue(device, vkb::QueueType::graphics);
    if (graphics_queue_result.isError()) {
        printError(graphics_queue_result.getError());
        return false;
    }
    auto graphics_queue = graphics_queue_result.getResult();

    auto presentation_queue_result =
        createQueue(device, vkb::QueueType::present);
    if (presentation_queue_result.isError()) {
        printError(presentation_queue_result.getError());
        return false;
    }
    auto presentation_queue = presentation_queue_result.getResult();

    auto allocator_result = createAllocator(instance, device);
    if (allocator_result.isError()) {
        printError(allocator_result.getError());
        return false;
    }
    auto allocator = allocator_result.getResult();

    loadExtensionFunctions();

    return true;
}

}  // namespace Graphics