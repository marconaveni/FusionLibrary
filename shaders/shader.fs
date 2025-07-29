#version 330 core
  
in vec4 ourColor;       // Cor recebida do Vertex Shader
in vec2 TexCoord;

uniform sampler2D ourTexture;
// uniform vec4 uvRegion; // Este uniform não está sendo usado, pode ser removido
// uniform vec4 tintColor; // Não precisamos mais dele, usaremos ourColor

out vec4 FragColor;

void main()
{
    // A linha mix() com uvRegion não está no seu código, então vou comentar
    // vec2 finalUV = mix(uvRegion.xy, uvRegion.zw, TexCoord);
    vec4 texColor = texture(ourTexture, TexCoord);

    // Combina a cor da textura com a cor do vértice (que inclui o alfa)
    FragColor = texColor * ourColor;
}