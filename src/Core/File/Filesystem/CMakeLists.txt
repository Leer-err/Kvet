target_sources(File PRIVATE
    Readers.cpp
    Filesystem.cpp
)

target_include_directories(File PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)