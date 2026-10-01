#pragma once

#include "Handle.h"

namespace Graphics {

class Texture;
class Mesh;
class GraphicsPipeline;

using TextureHandle = Handle<Texture>;
using MeshHandle = Handle<Mesh>;
using GraphicsPipelineHandle = Handle<GraphicsPipeline>;

}  // namespace Graphics