#version 330 core
  
in vec4 ourColor;       // Cor recebida do Vertex Shader
in vec2 TexCoord;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(ourTexture, TexCoord); // Pega a cor da textura normalmente
    // Combina a cor da textura com a cor do vértice (que inclui o alfa)
    FragColor = texColor * ourColor;
}