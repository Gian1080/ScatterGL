#version 460 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 normalColor;
layout (location = 2) out vec4 positionColor;

in vec3 FragmentPosition;
in vec3 Normal;
in vec2 texCoords;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shine; 
};
struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPosition;
uniform Material material;
uniform Light light;
uniform PointLight pointLight;

void main()
{
    normalColor = vec4(Normal, 1.0f);
    positionColor = vec4(FragmentPosition, 1.0f);
    //ambient light
    vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;

    //diffuse lighting
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, -light.direction), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

    //specular lighting
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(light.direction, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

    //point light calculations
    float distanceToSourceLight = length(pointLight.position - FragmentPosition);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distanceToSourceLight + 
                            pointLight.quadratic * (distanceToSourceLight * distanceToSourceLight));

    //combining results
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    vec3 result = diffuse + ambient + specular;
    FragColor = vec4(result, 1.0);
}