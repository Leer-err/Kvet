target_sources(Graphics PRIVATE
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Channel)