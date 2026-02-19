

target_sources(Graphics PRIVATE
    SwapChain.cpp
    SwapChainBuilder.cpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Private)