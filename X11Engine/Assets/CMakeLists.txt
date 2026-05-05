file(GLOB_RECURSE SHADERS "*.slang")

if(UNIX AND NOT APPLE)
    set(SLANG_COMPILER slangc)
elseif(WIN32)
    set(SLANG_COMPILER slangc.exe)
endif()

foreach(src_file ${SHADERS})
    get_filename_component(filename ${src_file} NAME_WE)
    get_filename_component(parent_folder ${src_file} DIRECTORY)
    get_filename_component(parent_folder ${parent_folder} NAME)
    set(out_file "${CMAKE_BINARY_DIR}/bin/Assets/Shaders/${parent_folder}/${filename}.spv")

    add_custom_command(
        OUTPUT ${out_file}
        COMMAND ${SLANG_COMPILER} ${src_file} -o ${out_file}
        DEPENDS ${src_file}
        COMMENT "Generating ${filename}.spv"
        VERBATIM
    )
    
    list(APPEND SHADER_OUTPUTS ${out_file})
endforeach()

add_custom_target(compile_shaders ALL DEPENDS ${SHADER_OUTPUTS})