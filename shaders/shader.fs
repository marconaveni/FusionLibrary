#version 330 core
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 uvRegion; // xy = canto inferior esquerdo, zw = canto superior direito
uniform vec4 tintColor;

out vec4 FragColor;

void main()
{
    vec2 finalUV = mix(uvRegion.xy, uvRegion.zw, TexCoord);
    vec4 texColor = texture(ourTexture, finalUV);
    FragColor = texColor * tintColor;
}