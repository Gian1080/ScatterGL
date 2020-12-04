#version 460 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrength;

void main()
{
    float ambientStrengthSecond = ambientStrength;
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = ambient * objectColor;
    //FragColor = vec4(lightColor * objectColor, 1.0);
    FragColor = vec4(result, 1.0);
}