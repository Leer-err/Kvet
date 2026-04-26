target_sources(Graphics PRIVATE
    Image.cpp
    ImageBuilder.cpp
)

target_include_directories(Graphics PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)