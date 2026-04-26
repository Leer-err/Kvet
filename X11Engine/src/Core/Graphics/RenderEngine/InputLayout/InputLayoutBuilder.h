#pragma once

#include <vulkan/vulkan_core.h>

#include <vector>

#include "InputLayout.h"

namespace Graphics {

class InputLayoutBuilder {
   public:
    InputLayoutBuilder();
    ~InputLayoutBuilder();

    InputLayoutBuilder& addElement(VkFormat format);
    InputLayoutBuilder& setPushConstantsSize(size_t size);

    InputLayout create();

   private:
    std::vector<VkFormat> elements;

    size_t push_constants_size;
};

}  // namespace Graphics