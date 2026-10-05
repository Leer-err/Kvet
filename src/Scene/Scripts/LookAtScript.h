#pragma once

#include <numbers>

#include "Camera.h"
#include "Entity.h"
#include "GameInputContext.h"
#include "Graphics.h"
#include "IScript.h"
#include "Quaternion.h"
#include "Transform.h"
#include "Vector3.h"
#include "WorldMatrix.h"

class LookAtScript final : public IScript {
   public:
    LookAtScript(const Entity& camera_entity, const Camera& camera,
                 const std::shared_ptr<Input::GameInputContext>& input)
        : camera(camera), camera_entity(camera_entity), input(input) {}

    void update(float delta_time) {
        float forward_backward =
            input->getAxis(Input::GameAxes::MoveForwardBackward);
        distance += forward_backward * delta_time;

        float pitch = input->getAxis(Input::GameAxes::LookPitch);
        float yaw = input->getAxis(Input::GameAxes::LookYaw);

        current_pitch += pitch;
        current_yaw += yaw;

        constexpr auto PI = std::numbers::pi_v<float>;
        constexpr auto HALF_PI = PI / 2;
        constexpr auto TWO_PI = PI * 2;
        if (current_pitch > HALF_PI)
            current_pitch = HALF_PI;
        else if (current_pitch < -HALF_PI)
            current_pitch = -HALF_PI;

        if (current_yaw > PI)
            current_yaw -= TWO_PI;
        else if (current_yaw < -PI)
            current_yaw += TWO_PI;

        auto rotation =
            Quaternion(0, current_yaw, 0) * Quaternion(current_pitch, 0, 0);
        auto camera_transform = camera_entity.get<Transform>();
        camera_transform->setOrientation(rotation);

        auto position = Vector3(0, 0, distance);
        position = position.rotate(rotation);

        camera_transform->setPosition(position);
    }

    void preRender() override {
        ZoneScoped;
        auto projection = camera.getProjectionMatrix();

        Vector3 position;
        Quaternion orientation;
        Vector3 scale;
        camera_entity.get<WorldMatrix>()->matrix.decompose(position, scale,
                                                           orientation);

        auto up = Vector3(0, 1, 0).rotate(orientation);
        auto forward = Vector3(0, 0, 1).rotate(orientation);
        auto right = Vector3(-1, 0, 0).rotate(orientation);

        Matrix view = Matrix::view(position, forward, up);
        Matrix view_camera_centered = Matrix::view(Vector3(), forward, up);

        auto& data =
            Graphics::getRenderEngine()->getRenderWorld()->renderData().camera;
        data.view_projection = projection * view;
        data.view_projection_camera_centered =
            projection * view_camera_centered;
        data.right = right;
        data.up = up;
        data.position = position;
    }

   private:
    float current_pitch = 0;
    float current_yaw = 0;
    float distance = -10;

    Camera camera;

    Entity camera_entity;

    std::shared_ptr<Input::GameInputContext> input;
};