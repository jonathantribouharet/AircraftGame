cmake_minimum_required(VERSION 2.6)
project("AircraftGame")

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)

if(APPLE)
	message(STATUS "Mac OS detected")

	file(GLOB_RECURSE sources *.c *.m)
else()
	message(STATUS "Linux detected")

	file(GLOB_RECURSE sources *.c)
endif()

if(CMAKE_COMPILER_IS_GNUCC)
	message(STATUS "Use gcc compiler")

	set(CMAKE_C_FLAGS "-Wall -O3 -std=c99 -s")
	#set(CMAKE_C_FLAGS "-Wall -DDEBUG -std=c99 ")
else()
	message(STATUS "Use clang compiler")

	set(CMAKE_C_FLAGS "-Wall -O3")
	#set(CMAKE_C_FLAGS "-Wall -DDEBUG")
endif()

include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDLIMAGE_INCLUDE_DIR})
include_directories(${SDLTTF_INCLUDE_DIR})

include_directories(.)
include_directories(Elements)
include_directories(Game)
include_directories(Other)

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY})
