add_library(Physics STATIC)

target_include_directories(Physics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Resources)
add_subdirectory(Shape)
add_subdirectory(RigidBody)
add_subdirectory(Scene)