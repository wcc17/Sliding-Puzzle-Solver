cmake_minimum_required(VERSION 3.3)
project(tilegame)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp TileGame.cpp TileGame.h Tile.cpp Tile.h Button.cpp Button.h Tree.cpp Tree.h Node.cpp Node.h MultiDimensionalTileArray.h)
add_executable(tilegame ${SOURCE_FILES})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED >=2.0.0)
#find_package(SDL_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})

target_link_libraries(tilegame ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})