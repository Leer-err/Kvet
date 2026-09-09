#pragma once

#include "Buffer.h"
#include "Handles.h"
#include "ResourceRegistry.h"
#include "Texture.h"


namespace Graphics {

using TextureRegistry = ResourceRegistry<Texture>;
using BufferRegistry = ResourceRegistry<Buffer>;

}  // namespace Graphics