#version 330 core
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 textColor;

out vec4 FragColor;

void main()
{
    float alpha = texture(ourTexture, TexCoord).r; // Pega o alfa do canal vermelho da textura da fonte
    FragColor = vec4(textColor.rgb, alpha * textColor.a); // branco com alpha da fonte
}
