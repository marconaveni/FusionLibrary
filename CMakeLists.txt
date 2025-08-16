cmake_minimum_required(VERSION 3.25)
project(FusionLibrary LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

# Opção de Compilação
option(FUSION_BUILD_FOR_WEB "Set to ON to build for WebAssembly with Emscripten" OFF)


set(COMMON_SOURCES
    src/core/core.cpp
    src/core/window.cpp
    # src/external/glad.c 
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

set(COMMON_INCLUDES
    include 
    src/core 
    src/external 
    src/graphics 
    src/platform 
    src/system
)

# Definição do Executável (movido para cima para que possamos definir propriedades nele)
add_executable(game
    # Os fontes serão adicionados depois
)

# Configuração Específica da Plataforma
if(FUSION_BUILD_FOR_WEB)
    message(STATUS "FusionLibrary: Compilando para a Web (WebAssembly)")
    
    # Linkamos contra a biblioteca 'GL' que o Emscripten fornece.
    set(PLATFORM_LIBS GL) 

    # Adiciona a definição para que o código C++ saiba que está na web
    target_compile_definitions(game PRIVATE FUSION_PLATFORM_WEB)
    
    # Adiciona as flags do Emscripten para habilitar WebGL2 (compatível com OpenGL ES 3.0)
    # Isso é essencial para que o GLAD encontre as funções.
    set_property(TARGET game APPEND_STRING PROPERTY LINK_FLAGS " -s FULL_ES3=1  -s USE_GLFW=3 ")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    set(PLATFORM_SOURCES 
        src/platform/platform_web.cpp
        libs/glad_web/src/glad.c
    )

    set(INCLUDE_GLAD
        libs/glad_web/include
    )

else()
    message(STATUS "FusionLibrary: Compilando para Desktop")

    #add_subdirectory(libs/glfw)

      # set(FUSION_GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw")   # opção B (com 's')

    if(EXISTS "${FUSION_GLFW_DIR}/CMakeLists.txt")
        message(STATUS "FusionLibrary: Using GLFW from submodule at ${FUSION_GLFW_DIR}")
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
        add_subdirectory("${FUSION_GLFW_DIR}")
    else()
        message(STATUS "FusionLibrary: GLFW submodule not found. Using FetchContent to retrieve GLFW")

        include(FetchContent)

        FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.9   
        )

        # Desliga artefatos que não precisamos
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

        FetchContent_MakeAvailable(glfw)
    endif()

    set(PLATFORM_LIBS glfw)

    
    set(PLATFORM_SOURCES
        src/platform/platform_desktop_glfw.cpp
        libs/glad_desktop/src/glad.c
    )
    
    set(INCLUDE_GLAD
        libs/glad_desktop/include
    )
endif()

# Adiciona todos os fontes ao executável
target_sources(game PRIVATE 
    ${COMMON_SOURCES}
    ${PLATFORM_SOURCES}
)

# Inclusão de Diretórios
target_include_directories(game PRIVATE
    ${COMMON_INCLUDES}
    ${INCLUDE_GLAD}
)

# Link de Bibliotecas
target_link_libraries(game PRIVATE ${PLATFORM_LIBS})

# Bibliotecas Específicas
if(WIN32 AND NOT FUSION_BUILD_FOR_WEB)
    target_link_libraries(game PRIVATE winmm)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT FUSION_BUILD_FOR_WEB)
    # Nenhuma biblioteca extra necessária.
endif()

