add_library(Graphics STATIC)

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
)

add_subdirectory(Renderer)
add_subdirectory(Internal)
add_subdirectory(GraphicsCommunicationManager)