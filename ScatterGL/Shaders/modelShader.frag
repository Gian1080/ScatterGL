#version 460 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 normalColor;
layout (location = 2) out vec4 positionColor;

in vec2 texCoords;
in vec3 normal;
in vec3 FragmentPosition;

uniform sampler2D texture_diffuse1;

void main()
{
    positionColor = vec4(FragmentPosition, 1.0);
    normalColor = vec4(normal, 1.0);
    FragColor = texture(texture_diffuse1, texCoords);
    if(FragColor.a <0.5)
    {
        discard;
    }
}