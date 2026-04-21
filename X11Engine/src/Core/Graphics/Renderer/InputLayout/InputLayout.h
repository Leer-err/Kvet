#pragma once

#include <memory>

#include "GraphicsInternalsForward.h"

namespace Graphics {

class InputLayout {
    friend class InputLayoutBuilder;
    friend class GraphicsPipelineBuilder;

   public:
    InputLayout();
    ~InputLayout();

    InputLayout(InputLayout&&);
    InputLayout& operator=(InputLayout&&);

   protected:
    InputLayout(Internal::InputLayout&& buffer);
    Internal::InputLayout* getInternal() const;

   private:
    std::unique_ptr<Internal::InputLayout> input_layout;
};

}  // namespace Graphics
