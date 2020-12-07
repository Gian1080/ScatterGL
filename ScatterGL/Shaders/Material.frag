#version 460 core
in vec3 Normal;
in vec3 FragmentPosition;

out vec4 FragColor;

uniform vec3 lightPosition;
uniform vec3 objectColor;
uniform vec3 lightColor;
//uniform float ambientStrength;

void main()
{
    //ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    //diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragmentPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;


    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}