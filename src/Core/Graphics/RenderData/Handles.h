#pragma once

#include "Handle.h"

namespace Graphics {

class Texture;
class Mesh;

using TextureHandle = Handle<Texture>;
using MeshHandle = Handle<Mesh>;

}  // namespace Graphics