# This file is indeed hacked together.
# So I'm not surprised if it doesn't work.
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
project(SnakeTheBullet)

# Set output directory
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Find packages
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/;${CMAKE_SOURCE_DIR}/cmake/Utils")

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)

# Optional parameters
set(CPPFLAGS "-O2" CACHE STRING "Compiler flags to use")
set(LINKFLAGS "" CACHE STRING "Linker flags to use")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKFLAGS}")

# Add additional compiler flags
add_definitions(${CPPFLAGS})
include_directories(${SDL_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})

# Add addition linker flags
link_libraries(
	${SDL_LIBRARY}
	${OPENGL_LIBRARIES}
	SDLmain
)
set(CMAKE_EXE_LINKER_FLAGS ${LINKFLAGS})

add_executable(SnakeTheBullet
	main.cpp
	
	game/game.cpp
	game/module.cpp
	game/signal.cpp
	game/timer.cpp
	
	game/player/player.cpp
	game/bullet/bullet.cpp
	
	)

