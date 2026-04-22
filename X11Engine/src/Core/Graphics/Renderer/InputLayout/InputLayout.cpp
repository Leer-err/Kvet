#include "InputLayout.h"

#include "GraphicsInternalsForward.h"
#include "InternalInputLayout.h"
#include "ResourceWrapper.h"

namespace Graphics {

InputLayout::InputLayout(const Internal::InputLayout& input_layout)
    : input_layout(
          std::make_shared<Internal::WrappedInputLayout>(input_layout)) {}

const Internal::InputLayout* InputLayout::getInternal() const {
    return input_layout->getPtr();
}

}  // namespace Graphics