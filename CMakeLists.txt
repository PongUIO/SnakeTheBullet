# This file is indeed hacked together.
# So I'm not surprised if it doesn't work.
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
project(SnakeTheBullet)

# Set output directory
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Find packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/;${CMAKE_SOURCE_DIR}/cmake/Utils")

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)

# Optional parameters
set(CPPFLAGS "-O2" CACHE STRING "Compiler flags to use")
set(LINKFLAGS "" CACHE STRING "Linker flags to use")
set(SCRSAVER OFF CACHE BOOL "Compile screensaver")
set(FRAMEBUFFER ON CACHE BOOL "Use framebuffer+shader rendering")
set(USE_PLAYERITEM ON CACHE BOOL "Implement the player and item module")
set(USE_BLUR ON CACHE BOOL "Use blurring")
set(TRAILPOWER 1250.0 CACHE STRING "Constant inverse factor to reduce background color per frame")
set(DELTATIME 0.01 CACHE STRING "Delta time per frame")
set(FPS 100 CACHE STRING "Frames per second")
set(BLURFOCUS 1.0 CACHE STRING "Level of focus on blurring")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKFLAGS}")

set(GS_THEME_WINDOW "")
if(SCRSAVER)
	find_package(GTK2 REQUIRED gtk)
	find_package(GtkGLExt REQUIRED)
	add_definitions("-DSCREENSAVER")
	link_libraries(${GTK2_LIBRARIES} ${K3D_GTKGLEXT_LIBS})
	include_directories(${GTK2_INCLUDE_DIRS} ${K3D_GTKGLEXT_INCLUDE_DIRS})
	
	set(GS_THEME_WINDOW "gs-theme-window.c")
	set(USE_PLAYERITEM OFF)
endif()

add_definitions("-DFPS_DEF=${FPS}")
add_definitions("-DDELTATIME=${DELTATIME}")

if(USE_BLUR)
	add_definitions("-DUSE_BLUR")
endif()

if(FRAMEBUFFER)
	add_definitions("-DFBO")
	add_definitions("-DTRAILPOWER=${TRAILPOWER}")
	add_definitions("-DBLURFOCUS=${BLURFOCUS}")
endif()

if(USE_PLAYERITEM)
	add_definitions("-DUSE_PLAYERITEM")
endif()

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
	${GS_THEME_WINDOW}
	
	game/game.cpp
	game/module.cpp
	game/signal.cpp
	game/timer.cpp
	game/fbo.cpp
	
	game/player/player.cpp
	
	game/item/item.cpp

	game/bullet/bullet.cpp
	game/bullet/brule.cpp
	game/bullet/state/CircleSpawn.cpp
	game/bullet/state/BulletFan.cpp
	game/bullet/state/MoveRandom.cpp
	game/bullet/state/ChangeMove.cpp
	game/bullet/state/IdleState.cpp
	
	game/controller/controller.cpp
	)

