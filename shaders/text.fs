#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    float alpha = texture(ourTexture, TexCoord).r;
    FragColor = vec4(1.0, 1.0, 1.0, alpha); // branco com alpha da fonte
}
