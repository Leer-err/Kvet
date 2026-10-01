#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "Result.h"

namespace Graphics {

class GraphicsPipeline {
   public:
    enum class Error { ShaderNotBuilt };

    static Result<GraphicsPipeline, Error> create(
        VkDevice device, const VkGraphicsPipelineCreateInfo& info) {
        VkPipeline pipeline;
        VkResult result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1,
                                                    &info, nullptr, &pipeline);

        return GraphicsPipeline(device, pipeline, info.layout);
    }

    GraphicsPipeline(VkDevice device, VkPipeline pipeline,
                     VkPipelineLayout layout)
        : device(device), pipeline(pipeline), layout(layout) {}
    ~GraphicsPipeline() {
        if (pipeline != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(device, layout, nullptr);
            vkDestroyPipeline(device, pipeline, nullptr);
        }
    }

    GraphicsPipeline(GraphicsPipeline&& other)
        : device(other.device), pipeline(other.pipeline), layout(other.layout) {
        other.pipeline = VK_NULL_HANDLE;
    }
    GraphicsPipeline& operator=(GraphicsPipeline&& other) {
        device = other.device;
        pipeline = other.pipeline;
        layout = other.layout;

        other.pipeline = VK_NULL_HANDLE;

        return *this;
    }
    GraphicsPipeline(const GraphicsPipeline& other) = delete;
    GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;

    VkPipeline getPipeline() const { return pipeline; }
    VkPipelineLayout getLayout() const { return layout; }

   private:
    VkDevice device;
    VkPipeline pipeline;
    VkPipelineLayout layout;
};

}  // namespace Graphics