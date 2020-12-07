#version 460 core
out vec4 FragColor;

in vec3 FragmentPosition;
in vec3 Normal;


uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    //ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse lighting
    //normalize(Normal);
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragmentPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular lighting
    float specularStrength = 0.5f;
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;


    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}