#pragma once

#include "Handles.h"
#include "MeshHandle.h"
#include "Quaternion.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Graphics {

struct Particle {
    Vector3 position;
    Vector3 velocity;
    Vector4 color;
    float size;
    float rotation;
    float lifetime;
    float age;
    uint32_t texture;
};

struct MeshParticle {
    Vector3 position;
    Quaternion orientation;
    float lifetime;
    float age;
    uint32_t texture;
    MeshHandle mesh;
};

}  // namespace Graphics