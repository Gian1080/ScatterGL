#version 460 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 texCoord;

layout (binding = 0) uniform sampler2D textureOne;
layout (binding = 1) uniform sampler2D textureTwo;

void main()
{
	FragColor = mix(texture(textureOne, texCoord), texture(textureTwo, texCoord), 0.2);
}