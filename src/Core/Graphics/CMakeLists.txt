add_library(Graphics STATIC)

target_sources(Graphics PRIVATE
    Graphics.cpp
    CustomRenderObject.cpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Graphics
    PUBLIC
        Window
        Logger
        Types
        Utility
        File
    PRIVATE 
        meshoptimizer
        tracy
        sdl
        ImGuiLib
        vulkan-interface
        vulkan-allocator
        vk-bootstrap::vk-bootstrap
        SPIRV-Reflect
)

add_subdirectory(RenderEngine)
add_subdirectory(RenderWorld)
add_subdirectory(Overlay)
add_subdirectory(Renderers)
add_subdirectory(RenderData)
add_subdirectory(VFX)
add_subdirectory(FrameGraph)
add_subdirectory(GraphicsResourceManager)
add_subdirectory(Include)