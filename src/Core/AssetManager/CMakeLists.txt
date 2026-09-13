add_library(AssetManager STATIC)

target_sources(AssetManager PRIVATE
    AssetManager.cpp
)

target_include_directories(AssetManager PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AssetManager 
    PUBLIC
        Utility
        Types
        Graphics
        File
)