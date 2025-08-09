cmake_minimum_required(VERSION 3.25)
project(FusionLibrary LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs/glfw) # se você tiver clonado GLFW

include_directories(include 
    ${CMAKE_SOURCE_DIR}/libs/glfw/include
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/external
    ${CMAKE_SOURCE_DIR}/src/graphics
    ${CMAKE_SOURCE_DIR}/src/platform
    ${CMAKE_SOURCE_DIR}/src/system
)



add_executable(game
    ${CMAKE_SOURCE_DIR}/src/core/window.cpp
    ${CMAKE_SOURCE_DIR}/src/external/glad.c
    ${CMAKE_SOURCE_DIR}/src/graphics/camera2d.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/font.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/render_texture.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/shader.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/sprite.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/text.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/texture.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/transformable.cpp
    ${CMAKE_SOURCE_DIR}/src/platform/platform_desktop_glfw.cpp
    ${CMAKE_SOURCE_DIR}/src/system/fusion_utf8.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)


target_link_libraries(game PRIVATE glfw ${CMAKE_DL_LIBS} )

# Para bibliotecas específicas de plataforma, como a winmm para Windows
if(WIN32)
    # A biblioteca winmm é necessária para funções como timeBeginPeriod
    target_link_libraries(game PRIVATE winmm)
endif()
