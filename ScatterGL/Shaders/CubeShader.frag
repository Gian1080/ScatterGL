#version 460 core

layout (binding = 0) uniform sampler2D textureOne;
layout (binding = 1) uniform sampler2D textureTwo;

in vec2 texCoord;

out vec4 FragColor;

void main()
{
	FragColor = mix(texture(textureOne, texCoord), texture(textureTwo, texCoord), 0.2);
}