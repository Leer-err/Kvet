#pragma once

#include "IScene.h"
#include "Sky.h"
#include "World.h"

class Scene : public IScene {
   public:
    Scene();

    void update(float deltaTime) override;

   private:
    void setupSystems();

    Sky sky;

    World world;
};