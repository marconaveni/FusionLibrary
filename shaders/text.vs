#version 330 core

// A entrada AGORA corresponde exatamente à configuração do batchVAO
layout (location = 0) in vec3 aPos;      // Posição do vértice
layout (location = 1) in vec4 aColor;    // Cor do vértice (mesmo que não usemos, precisa estar aqui)
layout (location = 2) in vec2 aTexCoord; // Coordenada da textura

uniform mat4 projection;

out vec2 TexCoord;
// Não precisamos passar a cor para o text.fs, ele usa um uniform

void main()
{
    // A posição já vem calculada, só aplicamos a projeção
    gl_Position = projection * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}