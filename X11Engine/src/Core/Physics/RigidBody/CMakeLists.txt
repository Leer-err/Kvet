

target_sources(Physics PRIVATE
    StaticRigidBody.cpp
    DynamicRigidBody.cpp
)

target_include_directories(Physics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)