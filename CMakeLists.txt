cmake_minimum_required(VERSION 3.10)
project(EngineOpengl)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/glfw) # se você tiver clonado GLFW
include_directories(include libs/glfw/include)

add_executable(game
    src/main.cpp
    src/glad.c
    src/shader.cpp
)

target_link_libraries(game glfw ${CMAKE_DL_LIBS})
