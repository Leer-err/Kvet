
add_subdirectory(Rasterizer)
add_subdirectory(Sampler)
add_subdirectory(GraphicsPipeline)
add_subdirectory(CommandPool)
add_subdirectory(CommandBuffer)
add_subdirectory(Shader)

target_include_directories(Graphics PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(Graphics PRIVATE
    Resources.cpp
)