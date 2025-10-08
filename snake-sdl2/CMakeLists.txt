cmake_minimum_required(VERSION 3.14)
project(snake-sdl2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(SDL2 QUIET sdl2)
endif()

find_path(SDL2_INCLUDE_DIR SDL.h PATH_SUFFIXES SDL2 include/SDL2 include)
find_library(SDL2_LIBRARY NAMES SDL2 SDL2main)

if (SDL2_INCLUDE_DIR AND SDL2_LIBRARY)
  include_directories(${SDL2_INCLUDE_DIR})
  set(SDL2_LIBS ${SDL2_LIBRARY})
elseif (SDL2_FOUND)
  include_directories(${SDL2_INCLUDE_DIRS})
  set(SDL2_LIBS ${SDL2_LIBRARIES})
else()
  message(FATAL_ERROR "SDL2 not found. Install libsdl2-dev or set SDL2 paths.")
endif()

add_executable(snake src/main.cpp)
target_link_libraries(snake ${SDL2_LIBS})