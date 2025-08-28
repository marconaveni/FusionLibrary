#ifndef SHADER_CODE
#define SHADER_CODE

#include <string>


namespace Fusion
{

#if !defined(FUSION_PLATFORM_WEB)
#define HEADER_TEXT "#version 330 core\n"
#else
#define HEADER_TEXT "#version 300 es\n    precision mediump float;\n"
#endif

    inline std::string GetDefaultVertexShader()
    {
        std::string vs = HEADER_TEXT;
        vs += R"(
layout (location = 0) in vec3 aPos;         // Posição já em coordenadas de mundo
layout (location = 1) in vec4 aColor;       // Cor do vértice (RGBA)
layout (location = 2) in vec2 aTexCoord;    // Coordenada da textura

out vec4 ourColor; // Saídas para o Fragment Shader
out vec2 TexCoord;

uniform mat4 projection; // Só precisamos da projeção, pois aPos já está no espaço do mundo
uniform mat4 view;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0); // Aplica apenas a projeção
    ourColor = aColor;
    TexCoord = aTexCoord;
}
)";
        return vs;
    }

    inline std::string GetDefaultFragmentShader()
    {

        std::string fs = HEADER_TEXT;
        fs += R"(       
in vec4 ourColor;       // Cor recebida do Vertex Shader
in vec2 TexCoord;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(ourTexture, TexCoord); // Pega a cor da textura normalmente  
    FragColor = texColor * ourColor;               // Combina a cor da textura com a cor do vértice (que inclui o alfa)
}
)";
        return fs;
    }

    inline std::string GetDefaultTextVertexShader()
    {
        std::string vs = HEADER_TEXT;
        vs += R"(
layout (location = 0) in vec3 aPos;      // Posição do vértice
layout (location = 1) in vec4 aColor;    // Cor do vértice (mesmo que não usemos, precisa estar aqui)
layout (location = 2) in vec2 aTexCoord; // Coordenada da textura

uniform mat4 projection;
uniform mat4 view;

out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);  // A posição já vem calculada, só aplicamos a projeção
    TexCoord = aTexCoord;
}
)";
        return vs;
    }

    inline std::string GetDefaultTextFragmentShader()
    {
        std::string fs = HEADER_TEXT;
        fs += R"(
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 textColor;

out vec4 FragColor;

void main()
{
    float alpha = texture(ourTexture, TexCoord).r;        // Pega o alfa do canal vermelho da textura da fonte
    FragColor = vec4(textColor.rgb, alpha * textColor.a); // branco com alpha da fonte
}
)";
        return fs;
    }


} // namespace Fusion

#ifdef HEADER_TEXT
#undef HEADER_TEXT
#endif


#endif // SHADER_CODE