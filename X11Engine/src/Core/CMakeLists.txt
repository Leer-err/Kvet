add_library(Core INTERFACE)

target_link_libraries(Core INTERFACE 
	Config
    Input
	Logger	
	Window
	Physics
	Script
)

add_subdirectory(Logger)
add_subdirectory(Input)
add_subdirectory(Config)
add_subdirectory(Window)
add_subdirectory(Physics)
add_subdirectory(Script)