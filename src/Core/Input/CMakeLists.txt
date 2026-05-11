add_library(Input STATIC)

target_sources(Input PRIVATE
    PhysicalInput.cpp
)

target_include_directories(Input PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)