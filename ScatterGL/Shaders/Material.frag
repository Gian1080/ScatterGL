#version 460 core
out vec4 FragColor;

in vec2 texCoords;
in vec3 FragmentPosition;
in vec3 Normal;



struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shine; //prefered numbers --> 1, 2, 4, 8 etc..
};
struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPosition;

void main()
{
    //ambient light
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    //diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragmentPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

    //specular lighting
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

    //combining results
    vec3 result = diffuse + ambient + specular;
    FragColor = vec4(result, 1.0);
}