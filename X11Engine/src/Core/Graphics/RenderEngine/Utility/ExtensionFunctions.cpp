#include "ExtensionFunctions.h"

#include "GraphicsResources.h"

namespace Graphics {

PFN_vkGetDescriptorSetLayoutSizeEXT vkGetDescriptorSetLayoutSizeEXT;
PFN_vkGetDescriptorSetLayoutBindingOffsetEXT
    vkGetDescriptorSetLayoutBindingOffsetEXT;
PFN_vkCmdBindDescriptorBuffersEXT vkCmdBindDescriptorBuffersEXT;
PFN_vkCmdSetDescriptorBufferOffsetsEXT vkCmdSetDescriptorBufferOffsetsEXT;
PFN_vkGetDescriptorEXT vkGetDescriptorEXT;
PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT
    vkCmdBindDescriptorBufferEmbeddedSamplersEXT;

void loadExtensionFunctions() {
    auto device = Resources::get().getDevice();

    vkGetDescriptorSetLayoutSizeEXT =
        reinterpret_cast<PFN_vkGetDescriptorSetLayoutSizeEXT>(
            vkGetDeviceProcAddr(device, "vkGetDescriptorSetLayoutSizeEXT"));
    vkGetDescriptorSetLayoutBindingOffsetEXT =
        reinterpret_cast<PFN_vkGetDescriptorSetLayoutBindingOffsetEXT>(
            vkGetDeviceProcAddr(device,
                                "vkGetDescriptorSetLayoutBindingOffsetEXT"));
    vkCmdBindDescriptorBuffersEXT =
        reinterpret_cast<PFN_vkCmdBindDescriptorBuffersEXT>(
            vkGetDeviceProcAddr(device, "vkCmdBindDescriptorBuffersEXT"));
    vkGetDescriptorEXT = reinterpret_cast<PFN_vkGetDescriptorEXT>(
        vkGetDeviceProcAddr(device, "vkGetDescriptorEXT"));
    vkCmdBindDescriptorBufferEmbeddedSamplersEXT =
        reinterpret_cast<PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT>(
            vkGetDeviceProcAddr(
                device, "vkCmdBindDescriptorBufferEmbeddedSamplersEXT"));
    vkCmdSetDescriptorBufferOffsetsEXT =
        reinterpret_cast<PFN_vkCmdSetDescriptorBufferOffsetsEXT>(
            vkGetDeviceProcAddr(device, "vkCmdSetDescriptorBufferOffsetsEXT"));
}

}  // namespace Graphics