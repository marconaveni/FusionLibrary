#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 uvRegion; // xy = canto inferior esquerdo, zw = canto superior direito

void main()
{
    vec2 finalUV = mix(uvRegion.xy, uvRegion.zw, TexCoord);
    FragColor = texture(ourTexture, finalUV);
}