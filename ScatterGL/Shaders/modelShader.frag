#version 460 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 normalColor;
layout (location = 2) out vec4 positionColor;

in vec3 FragmentPosition;
in vec3 Normal;
in vec2 texCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    positionColor = vec4(FragmentPosition, 1.0);
    normalColor = vec4(Normal, 1.0);
    FragColor = texture(texture_diffuse1, texCoords);
    if(FragColor.a <0.5)
    {
        discard;
    }
}