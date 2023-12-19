cmake_minimum_required(VERSION 3.26)
project(angry_birds)

set(CMAKE_CXX_STANDARD 17)

add_executable(angry_birds main.cpp object.cpp stage.cpp game.cpp)

target_link_libraries(angry_birds pthread ncursesw)
