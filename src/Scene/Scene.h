#pragma once

#include <string_view>

#include "Camera.h"
#include "IScene.h"
#include "Sky.h"
#include "World.h"

class Scene : public IScene {
   public:
    Scene();

    void update(float deltaTime) override;

   private:
    static void readEffect(std::string_view name);

    void setupSystems();

    Sky sky;

    Camera camera;

    World world;
};