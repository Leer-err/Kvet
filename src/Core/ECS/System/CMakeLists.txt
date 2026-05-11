

target_sources(ECS PRIVATE
    SystemDispatcher.cpp
)

target_include_directories(ECS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)