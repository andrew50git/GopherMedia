cmake_minimum_required(VERSION 3.4)

project(GopherMedia)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)
include_directories(/usr/include/SDL2 .)
link_directories(/usr/lib/x86_64-linux-gnu /usr/lib64)

add_library(SDL2 SHARED IMPORTED)
add_library(SDL2_image SHARED IMPORTED)
add_library(SDL2_ttf SHARED IMPORTED)
add_library(SDL2_mixer SHARED IMPORTED)
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libSDL2.so)
set_target_properties(SDL2_image PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libSDL2_image.so)
set_target_properties(SDL2_ttf PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libSDL2_ttf.so)
set_target_properties(SDL2_mixer PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libSDL2_mixer.so)

add_library(glew SHARED IMPORTED)
set_target_properties(glew PROPERTIES IMPORTED_LOCATION /usr/lib64/libGLEW.so)

add_library(gopher_media SHARED gopher_media.cpp)
target_link_libraries(gopher_media SDL2_image)
target_link_libraries(gopher_media SDL2_ttf)
target_link_libraries(gopher_media SDL2)
target_link_libraries(gopher_media SDL2_mixer)
target_link_libraries(gopher_media glew)

add_executable(test3d tests/test_3d.cpp)
add_executable(test2d tests/test_2d.cpp)

target_link_libraries(test3d gopher_media)
target_link_libraries(test2d gopher_media)
