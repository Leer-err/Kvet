#include "InputLayout.h"

#include "InternalInputLayout.h"

namespace Graphics {

InputLayout::~InputLayout() = default;

InputLayout::InputLayout(InputLayout&&) = default;

InputLayout& InputLayout::operator=(InputLayout&&) = default;

InputLayout::InputLayout() {
    input_layout = std::make_unique<Internal::InputLayout>();
}

InputLayout::InputLayout(Internal::InputLayout&& input_layout)
    : input_layout(
          std::make_unique<Internal::InputLayout>(std::move(input_layout))) {}

}  // namespace Graphics