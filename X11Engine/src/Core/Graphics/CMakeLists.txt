add_library(Graphics STATIC)

target_sources(Graphics PRIVATE
    Graphics.cpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Graphics
    PRIVATE 
        Window
        Logger
        Types
        Utility
        Tracy::TracyClient
        SDL3::SDL3
        vulkan-interface
        vulkan-allocator
        vk-bootstrap::vk-bootstrap
        spirv-cross-core
        spirv-cross-reflect
)

add_subdirectory(RenderEngine)
add_subdirectory(Renderers)
add_subdirectory(RenderData)
add_subdirectory(GraphicsCommunicationManager)