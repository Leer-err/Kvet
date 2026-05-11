add_library(Utility STATIC)

target_sources(Utility PRIVATE
    TypeIdHelper.cpp
)

target_include_directories(Utility PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)