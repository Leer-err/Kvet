#include "Stars.h"

#include <cstring>
#include <tracy/Tracy.hpp>

#include "Binding.h"

// #include "CameraManager.h"
#include "Engine.h"
#include "Matrix.h"
// #include "MeshBuilder.h"
// #include "Overlay.h"
#include "Vector3.h"

namespace StarData {
static constexpr Binding camera_parameters =
    Binding(Binding::ShaderType::Vertex, 0);
static constexpr Binding star_parameters =
    Binding(Binding::ShaderType::Pixel, 0);

struct CameraParameters {
    Matrix inverse_projection;
};

struct StarParameters {
    float time;
    float star_density;
    float blinking_speed;
    float blink_strength;
};

};  // namespace StarData

Stars::Stars() : star_density(30), blinking_speed(1), blink_strength(0.6) {}

void Stars::draw() {
    ZoneScoped;

    // auto star_parameters =
    // context.mapConstantBuffer<StarData::StarParameters>(
    //     star_parameters_buffer);
    // star_parameters->time = Engine::Engine::get().getTime();
    // star_parameters->star_density = star_density;
    // star_parameters->blink_strength = blink_strength;
    // star_parameters->blinking_speed = blinking_speed;
    // context.unmapConstantBuffer(star_parameters_buffer);

    // context.bindConstantBuffer(CameraManager::get().getCameraData(),
    //                            StarData::camera_parameters);
    // context.bindConstantBuffer(star_parameters_buffer,
    //                            StarData::star_parameters);

    // context.unbindConstantBuffer(StarData::camera_parameters);
    // context.unbindConstantBuffer(StarData::star_parameters);
}
