target_include_directories(Graphics PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Semaphore)
add_subdirectory(Fence)
add_subdirectory(CommandPool)
add_subdirectory(CommandBuffer)
add_subdirectory(ImageBarrier)
add_subdirectory(Pipeline)
add_subdirectory(RenderTarget)