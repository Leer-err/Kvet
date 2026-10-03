#pragma once

#include "Handle.h"
#include "Matrix.h"

namespace Graphics {

class CustomRenderObjectData;

class CustomRenderObject {
   public:
    explicit CustomRenderObject(const Handle<CustomRenderObjectData>& handle)
        : handle(handle) {}

    void updateData(const uint8_t* data, size_t size);
    void updateModelMatrix(const Matrix& model);

   private:
    Handle<CustomRenderObjectData> handle;
};

}  // namespace Graphics