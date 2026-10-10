cmake_minimum_required(VERSION 3.20)
project(spaceinvadersopengl LANGUAGES C CXX OBJCXX)

# macOS port only. The Windows game still builds from Space Invaders.sln.
if(NOT APPLE)
	message(FATAL_ERROR "CMakeLists.txt is the macOS port. On Windows, open Space Invaders.sln.")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(PREPEND CMAKE_PREFIX_PATH "/opt/homebrew")
find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)

add_executable(spaceinvaders
	platform/macos/host.cpp
	platform/macos/game.cpp
	platform/macos/runtime.c
	platform/macos/resources.c
	platform/macos/image.mm
	platform/macos/audio.c
	platform/macos/stubs.c
	platform/macos/stubs.cpp
)
target_compile_definitions(spaceinvaders PRIVATE GL_SILENCE_DEPRECATION)
target_include_directories(spaceinvaders BEFORE PRIVATE
	${CMAKE_SOURCE_DIR}/platform/macos/include
	${CMAKE_SOURCE_DIR}
)
target_link_libraries(spaceinvaders PRIVATE SDL2::SDL2 SDL2_mixer::SDL2_mixer
	"-framework OpenGL"
	"-framework ImageIO"
	"-framework CoreGraphics"
	"-framework CoreFoundation"
)

# Checklist item 2: compile the game. These objects are not linked yet.
set(GAME_SOURCES
	si_3DS.cpp
	si_3DS_MemoryFile.cpp
	si_3DS_Texture.cpp
	si_Basecode.cpp
	si_Font.c
	si_Fps.cpp
	si_Functions.cpp
	si_Loading.cpp
	si_Main.cpp
	si_Particles.cpp
	si_Resources.cpp
	si_Textures.cpp
	si_World.cpp
)
add_library(spaceinvaders_game OBJECT ${GAME_SOURCES})
target_include_directories(spaceinvaders_game BEFORE PRIVATE
	${CMAKE_SOURCE_DIR}/platform/macos/include
	${CMAKE_SOURCE_DIR}
)
target_compile_definitions(spaceinvaders_game PRIVATE
	GL_SILENCE_DEPRECATION
	WIN32
	_WIN32
	_WINDOWS
)
set_target_properties(spaceinvaders_game PROPERTIES
	CXX_STANDARD 98
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS ON
)
target_sources(spaceinvaders PRIVATE $<TARGET_OBJECTS:spaceinvaders_game>)

target_compile_options(spaceinvaders_game PRIVATE
	-Wno-unknown-pragmas
	-Wno-nonportable-include-path
	-Wno-c++11-compat-deprecated-writable-strings
	-Wno-extra-tokens
	-Wno-invalid-source-encoding
	-ferror-limit=0
)
