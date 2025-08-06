// outline.fs MODIFICADO

#version 330 core

// Input vertex attributes (from vertex shader)
in vec4 ourColor;
in vec2 TexCoord;

// Input uniform values
uniform sampler2D ourTexture; // <-- MUDANÇA AQUI

uniform vec2 textureSize;
uniform float outlineSize;
uniform vec4 outlineColor;

// Output fragment color
out vec4 FragColor;

void main()
{
    vec4 texel = texture(ourTexture, TexCoord);   // <-- MUDANÇA AQUI
    vec2 texelScale = vec2(0.0);
    texelScale.x = outlineSize/textureSize.x;
    texelScale.y = outlineSize/textureSize.y;

    // We sample four corner texels, but only for the alpha channel (this is for the outline)
    vec4 corners = vec4(0.0);
    corners.x = texture(ourTexture, TexCoord + vec2(texelScale.x, texelScale.y)).a; // <-- MUDANÇA AQUI
    corners.y = texture(ourTexture, TexCoord + vec2(texelScale.x, -texelScale.y)).a; // <-- MUDANÇA AQUI
    corners.z = texture(ourTexture, TexCoord + vec2(-texelScale.x, texelScale.y)).a; // <-- MUDANÇA AQUI
    corners.w = texture(ourTexture, TexCoord + vec2(-texelScale.x, -texelScale.y)).a; // <-- MUDANÇA AQUI

    float outline = min(dot(corners, vec4(1.0)), 1.0);
    vec4 color = mix(vec4(0.0), outlineColor, outline);
    //FragColor = mix(color, texel, texel.a);
    FragColor = mix(color, texel, texel.a);
}