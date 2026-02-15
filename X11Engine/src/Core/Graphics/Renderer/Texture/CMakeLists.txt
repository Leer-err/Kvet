target_sources(Graphics PRIVATE
    TextureBuilder.cpp
    Texture.cpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Private)