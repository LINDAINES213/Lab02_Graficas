cmake_minimum_required(VERSION 3.25)
project(Lab02_Graficas)

set(CMAKE_CXX_STANDARD 17)
set(SDL2_INCLUDE_DIR C:/libs/SDL/include)
set(SDL2_LIB_DIR C:/libs/SDL/lib/x64)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(Lab02_Graficas main.cpp)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2)