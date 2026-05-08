#pragma once

#include <vulkan/vulkan.h>

namespace Graphics {

void loadExtensionFunctions(VkDevice device);

extern PFN_vkGetDescriptorSetLayoutSizeEXT vkGetDescriptorSetLayoutSizeEXT;
extern PFN_vkGetDescriptorSetLayoutBindingOffsetEXT
    vkGetDescriptorSetLayoutBindingOffsetEXT;
extern PFN_vkCmdBindDescriptorBuffersEXT vkCmdBindDescriptorBuffersEXT;
extern PFN_vkCmdSetDescriptorBufferOffsetsEXT
    vkCmdSetDescriptorBufferOffsetsEXT;
extern PFN_vkGetDescriptorEXT vkGetDescriptorEXT;
extern PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT
    vkCmdBindDescriptorBufferEmbeddedSamplersEXT;

}  // namespace Graphics