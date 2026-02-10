target_sources(Graphics PRIVATE
    Buffer.cpp
    # BufferBuilder.cpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Private)