

target_sources(Physics PRIVATE
    Shape.cpp
)

target_include_directories(Physics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)