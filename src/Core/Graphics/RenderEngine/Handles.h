#pragma once

#include "Buffer.h"
#include "GraphicsMesh.h"
#include "Handle.h"
#include "Texture.h"

namespace Graphics {

using TextureHandle = Handle<Texture>;
using BufferHandle = Handle<Buffer>;
using MeshHandle = Handle<Mesh>;

}  // namespace Graphics