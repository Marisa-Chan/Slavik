cmake_minimum_required(VERSION 3.5)
project(slavik)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES 
	common/strutils.cpp
	fmt/format.cc
	fmt/os.cc
    cp866.cpp
	font.cpp
	fsmgr.cpp
	game.cpp
	game_arrows.cpp
	game_character.cpp
	game_data.cpp
	game_input.cpp
	game_item.cpp
	game_locale.cpp
	game_loot.cpp
	game_mixture_info.cpp
	game_newgame.cpp
	game_pathfind.cpp
	game_placeoff.cpp
	game_play.cpp
	game_quest.cpp
	game_sound.cpp
	game_soundres.cpp
	game_state.cpp
	game_trade.cpp
	game_ui.cpp
	game_village.cpp
	gfx.cpp
	glfuncs.cpp
	main.cpp
	resources.cpp
	system.cpp
)


find_package(PkgConfig REQUIRED)

pkg_search_module(SDL2 REQUIRED sdl2)
link_directories(${SDL2_LIBRARY_DIRS})

pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
link_directories(${SDL2_IMAGE_LIBRARY_DIRS})

find_package(OpenGL REQUIRED)

add_executable(slavik ${SOURCES})
target_link_libraries(slavik ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${OPENGL_LIBRARIES})
target_include_directories(slavik PUBLIC ${PROJECT_SOURCE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${OpenGL_INCLUDE_DIR})


if(CMAKE_BUILD_TYPE MATCHES [dD][eE][bB][uU][gG])
   target_compile_options(slavik PUBLIC "-g" "-O0")
   
   if(NOT WIN32)
      target_compile_options(slavik PUBLIC "-fsanitize=address")
      target_link_libraries(slavik "-fsanitize=address")
   endif()
else()
	target_compile_options(slavik PUBLIC "-O2" "-s")
endif()

