cmake_minimum_required(VERSION 3.25)
project(FusionLibrary LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)

# Opção de Compilação
option(FUSION_BUILD_FOR_WEB "Set to ON to build for WebAssembly with Emscripten" OFF)



file(GLOB_RECURSE FUSION_SOURCE_FILE "src/*.cpp")
file(GLOB_RECURSE FUSION_HEADER_FILE "src/*.h")

file(GLOB_RECURSE FUSION_CORE_SOURCE_FILE "src/core/*.cpp")
file(GLOB_RECURSE FUSION_CORE_HEADER_FILE "src/core/*.h")

file(GLOB_RECURSE FUSION_GRAPHICS_SOURCE_FILE "src/graphics/*.cpp")
file(GLOB_RECURSE FUSION_GRAPHICS_HEADER_FILE "src/graphics/*.h")

file(GLOB_RECURSE FUSION_PLATFORM_SOURCE_FILE "src/platform/*.cpp")
file(GLOB_RECURSE FUSION_PLATFORM_HEADER_FILE "src/platform/*.h")

file(GLOB_RECURSE FUSION_SYSTEM_SOURCE_FILE "src/system/*.cpp")
file(GLOB_RECURSE FUSION_SYSTEM_HEADER_FILE "src/system/*.h")

source_group("Core" FILES ${FUSION_CORE_SOURCE_FILE})
source_group("Core" FILES ${FUSION_CORE_HEADER_FILE})

source_group("Graphics" FILES ${FUSION_GRAPHICS_SOURCE_FILE})
source_group("Graphics" FILES ${FUSION_GRAPHICS_SOURCE_FILE})

source_group("Platform" FILES ${FUSION_PLATFORM_SOURCE_FILE})
source_group("Platform" FILES ${FUSION_PLATFORM_HEADER_FILE})

source_group("System" FILES ${FUSION_SYSTEM_SOURCE_FILE})
source_group("System" FILES ${FUSION_SYSTEM_HEADER_FILE})


# message(FATAL ${FUSION_SOURCE_FILE})

set(FUSION_COMMON_SOURCES
    src/core/core.cpp
    src/core/gamepad.cpp
    src/core/input.cpp
    src/core/keyboard.cpp
    src/core/mouse.cpp
    src/core/window.cpp
    src/graphics/camera2d.cpp
    src/graphics/font.cpp
    src/graphics/render_texture.cpp
    src/graphics/renderer.cpp
    src/graphics/shader.cpp
    src/graphics/sprite.cpp
    src/graphics/text.cpp
    src/graphics/texture.cpp
    src/graphics/transformable.cpp
    src/system/fusion_utf8.cpp
    src/main.cpp
)

set(FUSION_COMMON_INCLUDES
    "include"
    "libs/external"
    "src/core"
    "src/graphics"
    "src/platform"
    "src/system"
)

# Definição do Executável (movido para cima para que possamos definir propriedades nele)
add_executable(fusion
    # Os fontes serão adicionados depois
)

# Configuração Específica da Plataforma
if(FUSION_BUILD_FOR_WEB)
    message(STATUS "[FusionLibrary]: Compilando para a Web (WebAssembly)")
    
    # Linkamos contra a biblioteca 'GL' que o Emscripten fornece.
    set(FUSION_PLATFORM_LIBS GL) 

    # Adiciona a definição para que o código C++ saiba que está na web
    target_compile_definitions(fusion PRIVATE FUSION_PLATFORM_WEB)
    
    set_property(TARGET fusion APPEND_STRING PROPERTY LINK_FLAGS " -s FULL_ES3=1  -s USE_GLFW=3  --preload-file ../assets/ ")  # parametros estão hardcode deixar dinamico  
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    set(FUSION_PLATFORM_SOURCES 
        src/platform/platform_web.cpp
        libs/glad_web/src/glad.c
    )

    set(FUSION_INCLUDE_GLAD
        libs/glad_web/include
    )

    

else()

    message(STATUS "[FusionLibrary]: Compilando para Desktop")

    set(FUSION_GLFW_DIR ${CMAKE_SOURCE_DIR}/libs/glfw)

    if(EXISTS "${FUSION_GLFW_DIR}/CMakeLists.txt")
        message(STATUS "[FusionLibrary]: Using GLFW from submodule at ${FUSION_GLFW_DIR}")
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
        add_subdirectory("${FUSION_GLFW_DIR}")
    else()
        message(FATAL "[FusionLibrary]: GLFW submodule not found. try run comand "git submodule update --init --recursive"")
    endif()

    set(FUSION_PLATFORM_LIBS glfw)

    
    set(FUSION_PLATFORM_SOURCES
        src/platform/platform_desktop_glfw.cpp
        libs/glad_desktop/src/glad.c
    )
    
    set(FUSION_INCLUDE_GLAD
        libs/glad_desktop/include
    )
endif()

# Adiciona todos os fontes ao executável
target_sources(fusion PRIVATE 
    ${FUSION_COMMON_SOURCES}
    ${FUSION_PLATFORM_SOURCES}
)

# Inclusão de Diretórios
target_include_directories(fusion PRIVATE
    ${FUSION_COMMON_INCLUDES}
    ${FUSION_INCLUDE_GLAD}
)

# Link de Bibliotecas
target_link_libraries(fusion PRIVATE ${FUSION_PLATFORM_LIBS})

# Bibliotecas Específicas
if(WIN32 AND NOT FUSION_BUILD_FOR_WEB)
    target_link_libraries(fusion PRIVATE winmm)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT FUSION_BUILD_FOR_WEB)
    # Nenhuma biblioteca extra necessária.
endif()

