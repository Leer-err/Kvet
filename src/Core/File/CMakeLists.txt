add_library(File STATIC)

target_sources(File PRIVATE
)

target_include_directories(File PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(File
    PUBLIC
        Assimp
        Types
)

add_subdirectory(ModelReader)
