

target_sources(ECS PRIVATE
    EntityRegistry.cpp
    Entity.cpp
)

target_include_directories(ECS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)