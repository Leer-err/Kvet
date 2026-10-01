#pragma once

#include "Buffer.h"
#include "GraphicsMesh.h"
#include "GraphicsPipeline.h"
#include "ResourceRegistry.h"
#include "Texture.h"

namespace Graphics {

using TextureRegistry = ResourceRegistry<Texture>;
using BufferRegistry = ResourceRegistry<Buffer>;
using MeshRegistry = ResourceRegistry<Mesh>;
using PipelineRegistry = ResourceRegistry<GraphicsPipeline>;

}  // namespace Graphics