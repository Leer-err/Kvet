target_sources(Script PRIVATE
    CoreBindings.cpp
)

target_include_directories(Script PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Type)
add_subdirectory(Physics)