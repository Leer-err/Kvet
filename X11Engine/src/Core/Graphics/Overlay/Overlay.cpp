#include "Overlay.h"

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_vulkan.h>
#include <imgui.h>

#include <ranges>
#include <string>
#include <tracy/Tracy.hpp>

namespace Graphics {

OverlayElements::Text::Text(std::function<std::string()> text_callback)
    : text_callback(text_callback) {}

void OverlayElements::Text::draw() { ImGui::Text(text_callback().c_str()); }

OverlayElements::Header::Header(const std::string& label) : label(label) {}

void OverlayElements::Header::draw() {
    if (ImGui::CollapsingHeader(label.c_str())) {
        for (auto& child : elements) {
            std::visit([](auto& value) { value.draw(); }, child);
        }

        for (auto& [name, child] : headers) {
            child.draw();
        }
    }
}

OverlayElements::SliderFloat::SliderFloat(const std::string& label,
                                          std::function<void(float)> callback,
                                          float min, float max, float initial)
    : label(label),
      callback(callback),
      minimal(min),
      maximum(max),
      value(initial) {}

void OverlayElements::SliderFloat::draw() {
    bool is_changed =
        ImGui::SliderFloat(label.c_str(), &value, minimal, maximum);

    if (is_changed) callback(value);
}

Overlay::Overlay() {}

Overlay::~Overlay() {}

void Overlay::draw() {
    ZoneScoped;

    ImGui::Begin("Demo", nullptr, 0);

    for (auto& child : elements) {
        std::visit([](auto& value) { value.draw(); }, child);
    }

    for (auto& [name, child] : headers) {
        child.draw();
    }

    ImGui::End();
}

}  // namespace Graphics