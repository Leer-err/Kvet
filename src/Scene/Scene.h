#pragma once

#include <string_view>

#include "Camera.h"
#include "Filesystem.h"
#include "IScene.h"
#include "Sky.h"
#include "World.h"

class Scene : public IScene {
   public:
    Scene();

    void update(float deltaTime) override;

   private:
    void parseScene(const File::Scene& scene);

    void setupSystems();

    Sky sky;

    Camera camera;

    World world;
};