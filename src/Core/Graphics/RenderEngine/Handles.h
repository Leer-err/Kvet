#pragma once

#include "Handle.h"
#include "Texture.h"
#include "Buffer.h"

namespace Graphics {

using TextureHandle = Handle<Texture>;
using BufferHandle = Handle<Buffer>;

}  // namespace Graphics