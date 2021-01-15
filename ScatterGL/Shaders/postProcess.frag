#version 460 core
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D shadowTexture;
uniform sampler2D colorTexture;
uniform float intensity;
void main()
{
    vec4 shadowPixel = texture(shadowTexture, texCoords);
    vec4 colorPixel = texture(colorTexture, texCoords);
    FragColor = (colorPixel * shadowPixel) * vec4(intensity, intensity, intensity, 1.0f);
    if(FragColor.r == 0.0 &&  FragColor.g == 0.0  && FragColor.b == 0.0 )
    {
        FragColor = (colorPixel * 0.2) * vec4(intensity, intensity, intensity, 1.0f);
    }
}