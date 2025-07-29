
#version 330 core
layout (location = 0) in vec3 aPos;         // Posição já em coordenadas de mundo
layout (location = 1) in vec4 aColor;       // Cor do vértice (RGBA)
layout (location = 2) in vec2 aTexCoord;    // Coordenada da textura

// Saídas para o Fragment Shader
out vec4 ourColor;
out vec2 TexCoord;

// Só precisamos da projeção, pois aPos já está no espaço do mundo
uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aPos, 1.0); // Aplica apenas a projeção
    ourColor = aColor;
    TexCoord = aTexCoord;
}