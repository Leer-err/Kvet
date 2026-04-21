target_sources(Graphics PRIVATE
    InputLayout.cpp
    InputLayoutBuilder.cpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Private)