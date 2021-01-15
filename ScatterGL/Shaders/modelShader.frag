#version 460 core
layout (location = 0) out vec4 FragColor;


in vec2 texCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, texCoords);
    if(FragColor.a <0.5)
    {
        discard;
    }
}