#pragma once

#include <memory>

#include "AssetManager.h"
#include "Graphics.h"
#include "IScript.h"
#include "InputContext.h"
#include "PhysicalInput.h"

namespace Input {

enum class EditorActions { Reload };

enum class EditorAxes {
    LookPitch,
    LookYaw,
};

using EditorInputContext = InputContext<EditorActions, EditorAxes>;

}  // namespace Input

class ObjectManagerScript : public IScript {
   public:
    ObjectManagerScript() {
        input = std::make_shared<Input::EditorInputContext>();
        input->addBinding(Input::EditorActions::Reload,
                          Input::Button::KEYBOARD_R);

        handle = createRenderObject();
    }

    void update(float delta_time) {
        auto reload = input->getButton(Input::EditorActions::Reload);

        if (reload == Input::ButtonState::Pressed) {
            handle = createRenderObject();
            time = 0;
        }

        time += delta_time;
    }

    void preRender() override {
        handle.updateData(std::bit_cast<uint8_t*>(&time), sizeof(time));
    }

   private:
    static Graphics::CustomRenderObject createRenderObject() {
        auto custom_ro = Graphics::CustomRenderObjectCreateData{};
        custom_ro.mesh = Asset::Manager::getMesh("Base/Sphere").getResult();
        custom_ro.pipeline =
            Asset::Manager::getPipeline("Shaders/TestPipeline").getResult();
        custom_ro.model = Matrix::translation(Vector3{0, 0, 0});
        custom_ro.buffer_size = 4;

        return Graphics::getRenderEngine()
            ->getRenderWorld()
            ->addCustomRenderObject(custom_ro);
    }

    float time = 0;
    Graphics::CustomRenderObject handle;
    std::shared_ptr<Input::EditorInputContext> input;
};