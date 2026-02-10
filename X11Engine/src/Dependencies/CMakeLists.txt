cmake_minimum_required (VERSION 3.21)

include(FetchContent)
include(ExternalProject)

FetchContent_Declare(
	tracy
	GIT_REPOSITORY https://github.com/wolfpld/tracy.git
	GIT_TAG v0.12.2
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(tracy)

target_link_libraries(${TARGET_NAME} PUBLIC
	TracyClient
)

# add_custom_command(
# 	OUTPUT
#         ${CMAKE_BINARY_DIR}/bin/TracyClientd.dll
#     COMMAND 
# 		${CMAKE_COMMAND} -E copy ${tracy_BINARY_DIR}/TracyClientd.dll ${CMAKE_BINARY_DIR}/bin
#     DEPENDS ${EXECUTABLE_NAME}
# )
# add_custom_target(copy_tracy_libs ALL
#     DEPENDS ${CMAKE_BINARY_DIR}/bin/TracyClientd.dll
# )

#PhysX

set(PHYSX_BUILD_TYPE $<IF:$<CONFIG:Debug>,checked,profile>)

target_include_directories(${TARGET_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/PhysX/physx/include
)

target_link_directories(${TARGET_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/PhysX/physx/bin/win.x86_64.vc143.mt/${PHYSX_BUILD_TYPE}
	${CMAKE_CURRENT_SOURCE_DIR}/PhysX/physx/bin/linux.x86_64/${PHYSX_BUILD_TYPE}
)

target_link_libraries(${TARGET_NAME} PRIVATE
	PhysXExtensions_static_64
	PhysXPvdSDK_static_64
)

if (WIN32)
 	target_link_libraries(${TARGET_NAME} PRIVATE
    	PhysX_64
		PhysXCommon_64
		PhysXFoundation_64
 	)
else()
  	target_link_libraries(${TARGET_NAME} PRIVATE
    	PhysX_static_64
		PhysXCommon_static_64
		PhysXFoundation_static_64
	)
endif()

# add_custom_command(
# 	OUTPUT
#         ${CMAKE_BINARY_DIR}/bin/PhysX_64.dll
# 		${CMAKE_BINARY_DIR}/bin/PhysXFoundation_64.dll
# 		${CMAKE_BINARY_DIR}/bin/PhysXCommon_64.dll
#     COMMAND 
# 		${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/PhysX/physx/bin/win.x86_64.vc143.mt/${PHYSX_BUILD_TYPE}/PhysX_64.dll ${CMAKE_BINARY_DIR}/bin
# 	COMMAND
# 		${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/PhysX/physx/bin/win.x86_64.vc143.mt/${PHYSX_BUILD_TYPE}/PhysXFoundation_64.dll ${CMAKE_BINARY_DIR}/bin
# 	COMMAND
# 		${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/PhysX/physx/bin/win.x86_64.vc143.mt/${PHYSX_BUILD_TYPE}/PhysXCommon_64.dll ${CMAKE_BINARY_DIR}/bin
#     DEPENDS ${EXECUTABLE_NAME}
# )
# add_custom_target(copy_physx_libs ALL
#     DEPENDS ${CMAKE_BINARY_DIR}/bin/PhysX_64.dll
# 	DEPENDS ${CMAKE_BINARY_DIR}/bin/PhysXFoundation_64.dll
# )

#Assimp

# target_include_directories(${TARGET_NAME} PUBLIC
# 	${CMAKE_CURRENT_SOURCE_DIR}/assimp/include
# )

# target_link_directories(${TARGET_NAME} PUBLIC
# 	${CMAKE_CURRENT_SOURCE_DIR}/assimp/lib/Debug
# 	${CMAKE_CURRENT_SOURCE_DIR}/assimp/bin
# )

# add_custom_command(
# 	OUTPUT
#         ${CMAKE_BINARY_DIR}/bin/$<IF:$<CONFIG:Debug>,assimp-vc143-mtd.dll,assimp-vc143-mt.dll>
# 	COMMAND
# 		${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/assimp/bin/${CMAKE_BUILD_TYPE}/$<IF:$<CONFIG:Debug>,assimp-vc143-mtd.dll,assimp-vc143-mt.dll> ${CMAKE_BINARY_DIR}/bin
#     DEPENDS ${EXECUTABLE_NAME}
# )
# add_custom_target(copy_assimp_libs ALL
# 	DEPENDS ${CMAKE_BINARY_DIR}/bin/$<IF:$<CONFIG:Debug>,assimp-vc143-mtd.dll,assimp-vc143-mt.dll>
# )

# ImGui

add_library(ImGuiLib STATIC)

target_include_directories(ImGuiLib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/
)

target_sources(ImGuiLib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
)

if(WIN32)
target_sources(ImGuiLib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_win32.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_dx11.cpp
)
endif()

target_link_libraries(${TARGET_NAME} PUBLIC
    ImGuiLib
)

#Lua

target_include_directories(${TARGET_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/lua/include
)

target_link_directories(${TARGET_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/lua
)

find_library(LUA_PATH PATH liblua54.a HINTS ${CMAKE_CURRENT_SOURCE_DIR}/lua/)

target_link_libraries(${TARGET_NAME} PUBLIC 
	${LUA_PATH}
)

# add_custom_command(
# 	OUTPUT
#         ${CMAKE_BINARY_DIR}/bin/lua54.dll
# 	COMMAND
# 		${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/lua/lua54.dll ${CMAKE_BINARY_DIR}/bin
#     DEPENDS ${EXECUTABLE_NAME}
# )
# add_custom_target(copy_lua_libs ALL
# 	DEPENDS ${CMAKE_BINARY_DIR}/bin/lua54.dll
# )

# glm

add_subdirectory(glm EXCLUDE_FROM_ALL)

target_link_libraries(${TARGET_NAME} PRIVATE glm::glm)

# SDL3

add_subdirectory(sdl EXCLUDE_FROM_ALL)

# vk-bootstrap

add_subdirectory(vk-bootstrap)

# Vulkan

add_library(vulkan-interface INTERFACE)

if (WIN32)
 	target_include_directories(vulkan-interface INTERFACE
		${VULKAN_PATH}/include
	)
	target_link_directories(vulkan-interface INTERFACE
		${VULKAN_PATH}/Lib
	)
	target_link_libraries(vulkan-interface INTERFACE vulkan-1)
else()
	find_package(Vulkan REQUIRED)
  	target_link_libraries(vulkan-interface INTERFACE Vulkan::Vulkan)
endif()