target_sources(Graphics PRIVATE
    GraphicsPipeline.cpp
    GraphicsPipelineBuilder.cpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)