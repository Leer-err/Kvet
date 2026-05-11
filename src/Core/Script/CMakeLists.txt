add_library(Script STATIC)

target_sources(Script PRIVATE
    ScriptResources.cpp
)

target_include_directories(Script PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Bindings)