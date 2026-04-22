#pragma once

#include <memory>

#include "GraphicsInternalsForward.h"

namespace Graphics {

class InputLayout {
    friend class InputLayoutBuilder;
    friend class GraphicsPipelineBuilder;

   public:
   protected:
    InputLayout(const Internal::InputLayout& buffer);
    const Internal::InputLayout* getInternal() const;

   private:
    std::shared_ptr<Internal::WrappedInputLayout> input_layout;
};

}  // namespace Graphics
