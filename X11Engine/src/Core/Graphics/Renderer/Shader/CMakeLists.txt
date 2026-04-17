target_sources(Graphics PRIVATE
    Shader.cpp
    ShaderBuilder.cpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Private)
add_subdirectory(ShaderRegistry)