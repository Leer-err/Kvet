#pragma once

#include <cstdint>
#include <vector>

#include "Handles.h"
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

struct RibbonNode {
    Vector3 position;
};

struct Ribbon {
    GraphicsPipelineHandle pipeline;
    std::vector<uint32_t> nodes;
};

}  // namespace Graphics