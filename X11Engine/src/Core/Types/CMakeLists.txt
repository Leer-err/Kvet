add_library(Types STATIC)

target_sources(Types PRIVATE
    Transform.cpp
)

target_include_directories(Types PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Types
    PRIVATE
        physx-lib
        glm::glm
)

add_subdirectory(Result)
add_subdirectory(Math)
