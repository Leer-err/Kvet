target_sources(Graphics PRIVATE
    RenderEngine.cpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Resources)
add_subdirectory(Image)
add_subdirectory(SwapChain)
add_subdirectory(Shader)
add_subdirectory(Buffer)
add_subdirectory(RenderTarget)
add_subdirectory(RenderEnviroment)
add_subdirectory(Semaphore)
add_subdirectory(Fence)
add_subdirectory(CommandPool)
add_subdirectory(CommandBuffer)
add_subdirectory(DepthStencil)
add_subdirectory(Binding)
add_subdirectory(InputLayout)
add_subdirectory(GraphicsPipeline)
add_subdirectory(DeviceProperties)
add_subdirectory(DescriptorSet)
add_subdirectory(TextureView)
add_subdirectory(Sampler)
add_subdirectory(Rasterizer)
add_subdirectory(Utility)