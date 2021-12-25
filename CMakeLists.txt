cmake_minimum_required(VERSION 3.0.0)

project(GopherMedia)

set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "cmake/")

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." ON)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options(-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options(-fcolor-diagnostics)
    endif()
endif()


find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_MIXER_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(src/)

add_library(gopher_media SHARED src/basic.cpp src/base_game.cpp src/game_2d.cpp src/game_3d.cpp)
target_link_libraries(gopher_media ${SDL2_LIBRARIES})
target_link_libraries(gopher_media ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(gopher_media ${SDL2_TTF_LIBRARIES})
target_link_libraries(gopher_media ${SDL2_MIXER_LIBRARIES})
target_link_libraries(gopher_media ${GLEW_LIBRARIES})

add_executable(test3d tests/test_3d.cpp)
add_executable(test2d tests/test_2d.cpp)

target_link_libraries(test3d gopher_media)
target_link_libraries(test2d gopher_media)
