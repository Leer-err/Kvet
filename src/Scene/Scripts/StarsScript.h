#pragma once

#include "AssetManager.h"
#include "Graphics.h"
#include "IScript.h"
#include "Matrix.h"

class StarsScript : public IScript {
    struct StarsData {
        float time = 0;
        float star_density = 30;
        float blinking_speed = 1;
        float blinking_strength = 0.6;
    };

   public:
    StarsScript() {
        auto custom_ro = Graphics::CustomRenderObjectCreateData{};
        custom_ro.mesh = Asset::Manager::getMesh("Base/SkySphere").getResult();
        custom_ro.pipeline =
            Asset::Manager::getPipeline("Shaders/StarPipeline").getResult();
        custom_ro.buffer_size = sizeof(StarsData);

        sphere = Graphics::getRenderEngine()
                     ->getRenderWorld()
                     ->addCustomRenderObject(custom_ro);

        sphere.updateData(std::bit_cast<uint8_t*>(&data), sizeof(StarsData));
    }

    void update(float delta_time) override { data.time += delta_time; }

    void preRender() override {
        sphere.updateData(std::bit_cast<uint8_t*>(&data), sizeof(StarsData));
    }

   private:
    StarsData data;

    Graphics::CustomRenderObject sphere;
};