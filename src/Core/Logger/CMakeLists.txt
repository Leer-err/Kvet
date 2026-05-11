include(FetchContent)

FetchContent_Declare(spdlog 
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.1
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(spdlog)

add_library(Logger STATIC)

target_link_libraries(Logger PRIVATE
    spdlog::spdlog_header_only
)

target_sources(Logger PRIVATE
    Logger.cpp
    LoggerFactory.cpp
)

set_target_properties(Logger PROPERTIES
    CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

target_include_directories(Logger 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${spdlog_SOURCE_DIR}/include
)