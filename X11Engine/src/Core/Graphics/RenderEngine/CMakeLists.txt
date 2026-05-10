target_sources(Graphics PRIVATE
    RenderEngine.cpp
)

target_include_directories(Graphics PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Device)
add_subdirectory(SwapChain)
add_subdirectory(Mesh)
add_subdirectory(RenderEnviroment)
add_subdirectory(DeviceProperties)
add_subdirectory(DescriptorSet)
add_subdirectory(ShaderRegistry)
add_subdirectory(Utility)
add_subdirectory(AdapterTypes)
add_subdirectory(StagingBuffer)