#version 460 core

out vec4 fragColor;

uniform float nearPlane;
uniform float farPlane;

float near = 0.1;
float far = 100.0;

float lineraizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; //normalize depths
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = lineraizeDepth(gl_FragCoord.z) / far;
    fragColor = vec4(vec3(depth), 1.0);
}