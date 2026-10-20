#include "OverlayRenderer.h"

#include "EngineData.h"
#include "Overlay.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_vulkan.h"

namespace Graphics {

OverlayRenderer::OverlayRenderer(const EngineData& engine_data)
    : engine_data(engine_data) {}

void OverlayRenderer::render(FrameGraph& frame_graph) {
    auto color_attachment =
        *engine_data.resource_manager.getTexture("RenderResult");

    auto pass =
        GeneralPass("Ribbon", [color_attachment](const CommandBuffer& cmd) {
            RenderEnviroment env = {};

            env.render_target = color_attachment->getView();
            env.width = color_attachment->getWidth();
            env.height = color_attachment->getHeight();

            cmd.bindRenderEnviroment(env);

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            Overlay::Overlay::get().draw();

            ImGui::Render();
            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd.buffer);

            cmd.unbindRenderEnviroment();
        });

    frame_graph.addGeneralPass(pass);
}

}  // namespace Graphics