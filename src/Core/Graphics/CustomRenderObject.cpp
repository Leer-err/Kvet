#include "CustomRenderObject.h"

#include "Graphics.h"
#include "RenderWorld.h"

namespace Graphics {

void CustomRenderObject::updateData(const uint8_t* data, size_t size) {
    handle->buffer->update(data, size);
}

void CustomRenderObject::updateModelMatrix(const Matrix& model) {
    handle->model = model;
}

}  // namespace Graphics