add_library(Window STATIC)

target_sources(Window PRIVATE
    Window.cpp
)

target_include_directories(Window PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)