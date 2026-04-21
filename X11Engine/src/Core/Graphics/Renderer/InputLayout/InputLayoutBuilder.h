#pragma once

#include <d3d11_1.h>
#include <dxgi1_4.h>

#include <string>
#include <vector>

#include "InputLayout.h"

namespace Graphics {

enum class InputElementFormat { Vector3f };

class InputLayoutBuilder {
   public:
    InputLayoutBuilder();
    ~InputLayoutBuilder();

    InputLayoutBuilder& addElement(InputElementFormat format);

    InputLayout create();

   private:
    std::vector<InputElementFormat> elements;
};

}  // namespace Graphics