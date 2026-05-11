target_sources(Physics PRIVATE
    PhysicsScene.cpp
)

target_include_directories(Physics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)