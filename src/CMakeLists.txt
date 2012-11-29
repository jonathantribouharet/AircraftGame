cmake_minimum_required(VERSION 2.6)
project("AircraftGame")

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)

if(APPLE)
	message(STATUS "Mac OS detected")
	message(STATUS "Use clang compiler")

	set(CMAKE_C_FLAGS "-O3 -Wall")
	#set(CMAKE_C_FLAGS "-Wall")

	include_directories("/System/Library/Frameworks/SDL.framework/headers"
					    "/System/Library/Frameworks/SDL_ttf.framework/headers"
					    "/System/Library/Frameworks/SDL_image.framework/headers")

	file(GLOB_RECURSE sources *.c *.m)
else()
	message(STATUS "Use gcc compiler")

	set(CMAKE_C_FLAGS "-O3 -Wall -s")
	#set(CMAKE_C_FLAGS "-Wall")

	file(GLOB_RECURSE sources *.c)
endif()

include_directories(.)
include_directories(Elements)
include_directories(Game)
include_directories(Other)

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY})
