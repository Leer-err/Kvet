#pragma once

#include "Buffer.h"

namespace Graphics {

struct Mesh {
    Buffer index_buffer;
    Buffer vertex_buffer;
};

}  // namespace Graphics