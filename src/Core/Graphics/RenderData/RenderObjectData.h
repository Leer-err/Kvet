#pragma once

#include "Handles.h"
#include "Vector3.h"

struct RenderObjectData {
    Vector3 position;

    Graphics::TextureHandle albedo;
    Graphics::MeshHandle mesh;
};