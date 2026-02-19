target_sources(Graphics PRIVATE
    Renderer.cpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Resources)
# add_subdirectory(Context)
add_subdirectory(SwapChain)
# add_subdirectory(Shader)
add_subdirectory(Texture)
add_subdirectory(Buffer)
# add_subdirectory(RenderTarget)
# add_subdirectory(DepthStencil)
# add_subdirectory(Binding)
# add_subdirectory(InputLayout)
# add_subdirectory(GraphicsPipeline)
# add_subdirectory(ShaderResource)
# add_subdirectory(Sampler)
# add_subdirectory(Rasterizer)